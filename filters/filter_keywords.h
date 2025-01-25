//
// Created by cvrain on 25-1-24.
//

#ifndef FILTER_KEYWORDS_H
#define FILTER_KEYWORDS_H

namespace filter {
    class keywords final{
    public:
        static constexpr auto user_id_exist = "filter::UserIdExist";
        static constexpr auto user_email_exist = "filter::UserEmailExist";
        static constexpr auto user_auth = "filter::UserAuth";
    };
}

#endif //FILTER_KEYWORDS_H
