// httpclient.h

/*    Copyright 2009 10gen Inc.
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

#pragma once

#include <map>
#include <string>

#include <boost/noncopyable.hpp>

#include "mongo/client/export_macros.h"

namespace mongo {

    class MONGO_CLIENT_API HttpClient : boost::noncopyable {
    public:

        typedef std::map<std::string,std::string> Headers;

        class MONGO_CLIENT_API Result {
        public:
            Result() {}

            const std::string& getEntireResponse() const {
                return _entireResponse;
            }

            Headers getHeaders() const {
                return _headers;
            }

            const std::string& getBody() const {
                return _body;
            }

        private:

            void _init( int code , std::string entire );

            int _code;
            std::string _entireResponse;

            Headers _headers;
            std::string _body;

            friend class HttpClient;
        };

        /**
         * @return response code
         */
        int get( const std::string& url , Result * result = 0 );

        /**
         * @return response code
         */
        int post( const std::string& url , const std::string& body , Result * result = 0 );

    private:
        int _go( const char * command , std::string url , const char * body , Result * result );
    };
}
