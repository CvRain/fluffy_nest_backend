//
// Created by cvrain on 25-1-24.
//

#ifndef FILTER_KEYWORDS_H
#define FILTER_KEYWORDS_H

namespace filter {
    class keywords final{
    public:
        static constexpr auto user_email_exist = "filter::UserEmailExistMiddleware";
        static constexpr auto user_id_exist = "filter::UserIdExistMiddleware";
        static constexpr auto user_name_exist = "filter::UserNameExistMiddleware";
        static constexpr auto user_auth = "filter::UserAuth";
        static constexpr auto enable_cores = "filter::EnableCores";
        static constexpr auto enable_cores_v2 = "filter::EnableCoresV2";
        static constexpr auto allow_intranet_access = "drogon::IntranetIpFilter";
        static constexpr auto allow_local_host = "drogon::LocalHostFilter";
    };
}

#endif //FILTER_KEYWORDS_H
