/*    Copyright 2014 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "mongo/db/jsobj.h"

#include <boost/lexical_cast.hpp>

#include "mongo/bson/optime.h"
#include "mongo/util/log.h"

namespace mongo {

    using std::endl;
    using std::numeric_limits;
    using std::string;

    void BSONObjBuilder::appendMinForType( const StringData& fieldName , int t ) {
        switch ( t ) {
                
        // Shared canonical types
        case NumberInt:
        case NumberDouble:
        case NumberLong:
            append( fieldName , - numeric_limits<double>::max() ); return;
        case Symbol:
        case String:
            append( fieldName , "" ); return;
        case Date: 
            // min varies with V0 and V1 indexes, so we go one type lower.
            appendBool(fieldName, true);
            //appendDate( fieldName , numeric_limits<long long>::min() ); 
            return;
        case Timestamp: // TODO integrate with Date SERVER-3304
            appendTimestamp( fieldName , 0 ); return;
        case Undefined: // shared with EOO
            appendUndefined( fieldName ); return;
                
        // Separate canonical types
        case MinKey:
            appendMinKey( fieldName ); return;
        case MaxKey:
            appendMaxKey( fieldName ); return;
        case jstOID: {
            OID o;
            memset(&o, 0, sizeof(o));
            appendOID( fieldName , &o);
            return;
        }
        case Bool:
            appendBool( fieldName , false); return;
        case jstNULL:
            appendNull( fieldName ); return;
        case Object:
            append( fieldName , BSONObj() ); return;
        case Array:
            appendArray( fieldName , BSONObj() ); return;
        case BinData:
            appendBinData( fieldName , 0 , BinDataGeneral , (const char *) 0 ); return;
        case RegEx:
            appendRegex( fieldName , "" ); return;
        case DBRef: {
            OID o;
            memset(&o, 0, sizeof(o));
            appendDBRef( fieldName , "" , o );
            return;
        }
        case Code:
            appendCode( fieldName , "" ); return;
        case CodeWScope:
            appendCodeWScope( fieldName , "" , BSONObj() ); return;
        };
        log() << "type not supported for appendMinElementForType: " << t << endl;
        uassert( 10061 ,  "type not supported for appendMinElementForType" , false );
    }

    void BSONObjBuilder::appendMaxForType( const StringData& fieldName , int t ) {
        switch ( t ) {
                
        // Shared canonical types
        case NumberInt:
        case NumberDouble:
        case NumberLong:
            append( fieldName , numeric_limits<double>::max() ); return;
        case Symbol:
        case String:
            appendMinForType( fieldName, Object ); return;
        case Date:
            appendDate( fieldName , numeric_limits<long long>::max() ); return;
        case Timestamp: // TODO integrate with Date SERVER-3304
            append( fieldName , OpTime::max() ); return;
        case Undefined: // shared with EOO
            appendUndefined( fieldName ); return;

        // Separate canonical types
        case MinKey:
            appendMinKey( fieldName ); return;
        case MaxKey:
            appendMaxKey( fieldName ); return;
        case jstOID: {
            OID o;
            memset(&o, 0xFF, sizeof(o));
            appendOID( fieldName , &o);
            return;
        }
        case Bool:
            appendBool( fieldName , true ); return;
        case jstNULL:
            appendNull( fieldName ); return;
        case Object:
            appendMinForType( fieldName, Array ); return;
        case Array:
            appendMinForType( fieldName, BinData ); return;
        case BinData:
            appendMinForType( fieldName, jstOID ); return;
        case RegEx:
            appendMinForType( fieldName, DBRef ); return;
        case DBRef:
            appendMinForType( fieldName, Code ); return;                
        case Code:
            appendMinForType( fieldName, CodeWScope ); return;
        case CodeWScope:
            // This upper bound may change if a new bson type is added.
            appendMinForType( fieldName , MaxKey ); return;
        }
        log() << "type not supported for appendMaxElementForType: " << t << endl;
        uassert( 14853 ,  "type not supported for appendMaxElementForType" , false );
    }

    bool BSONObjBuilder::appendAsNumber( const StringData& fieldName , const string& data ) {
        if ( data.size() == 0 || data == "-" || data == ".")
            return false;

        unsigned int pos=0;
        if ( data[0] == '-' )
            pos++;

        bool hasDec = false;

        for ( ; pos<data.size(); pos++ ) {
            if ( isdigit(data[pos]) )
                continue;

            if ( data[pos] == '.' ) {
                if ( hasDec )
                    return false;
                hasDec = true;
                continue;
            }

            return false;
        }

        if ( hasDec ) {
            double d = atof( data.c_str() );
            append( fieldName , d );
            return true;
        }

        if ( data.size() < 8 ) {
            append( fieldName , atoi( data.c_str() ) );
            return true;
        }

        try {
            long long num = boost::lexical_cast<long long>( data );
            append( fieldName , num );
            return true;
        }
        catch(boost::bad_lexical_cast &) {
            return false;
        }
    }

    /* add all the fields from the object specified to this object */
    BSONObjBuilder& BSONObjBuilder::appendElements(BSONObj x) {
        if (!x.isEmpty())
            _b.appendBuf(
                x.objdata() + 4,   // skip over leading length
                x.objsize() - 5);  // ignore leading length and trailing \0
        return *this;
    }

    /* add all the fields from the object specified to this object if they don't exist */
    BSONObjBuilder& BSONObjBuilder::appendElementsUnique(BSONObj x) {
        std::set<std::string> have;
        {
            BSONObjIterator i = iterator();
            while ( i.more() )
                have.insert( i.next().fieldName() );
        }
        
        BSONObjIterator it(x);
        while ( it.more() ) {
            BSONElement e = it.next();
            if ( have.count( e.fieldName() ) )
                continue;
            append(e);
        }
        return *this;
    }

    void BSONObjBuilder::appendKeys( const BSONObj& keyPattern , const BSONObj& values ) {
        BSONObjIterator i(keyPattern);
        BSONObjIterator j(values);

        while ( i.more() && j.more() ) {
            appendAs( j.next() , i.next().fieldName() );
        }

        verify( ! i.more() );
        verify( ! j.more() );
    }

    BSONObjIterator BSONObjBuilder::iterator() const {
        const char * s = _b.buf() + _offset;
        const char * e = _b.buf() + _b.len();
        return BSONObjIterator( s , e );
    }

    bool BSONObjBuilder::hasField( const StringData& name ) const {
        BSONObjIterator i = iterator();
        while ( i.more() )
            if ( name == i.next().fieldName() )
                return true;
        return false;
    }

} // namespace mongo
