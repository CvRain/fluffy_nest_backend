//
// Created by cvrain on 24-8-19.
//

#include "string.hpp"

#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>

#include "utils/date.h"

namespace fluffy_utils {
    std::string StringEncryption::secret_string = "none-secret";

    /**
     * @brief 计算SHA-256哈希值
     * @param str 待计算散列值的原始数据字符串
     * @return SHA-256哈希值的字符串表示
     */
    std::string StringEncryption::sha256(const std::string &str) {
        // 定义一个数组用于存储SHA-256哈希摘要
        unsigned char hash[SHA256_DIGEST_LENGTH];
        // 创建并初始化一个EVP_MD_CTX上下文对象，用于执行哈希操作
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        // 初始化上下文对象，指定使用SHA-256哈希算法
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        // 更新哈希操作，将输入字符串str的数据纳入计算
        EVP_DigestUpdate(ctx, str.c_str(), str.size());
        // 定义一个变量用于存储哈希摘要的长度
        unsigned int len;
        // 完成哈希计算，并将结果存储到hash数组中
        EVP_DigestFinal_ex(ctx, hash, &len);
        // 释放上下文对象，清理资源
        EVP_MD_CTX_free(ctx);

        // 创建一个字符串流对象，用于构建最终的哈希字符串
        std::stringstream ss;
        // 遍历hash数组中的每个字节，并以十六进制格式输出
        for (const unsigned char i: hash) {
            // 将每个字节转换为两位十六进制数，并添加到字符串流中
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }

        // 返回构建好的SHA-256哈希字符串
        return ss.str();
    }

    /**
     * @brief 计算SHA-512哈希值
     * @param str 待计算散列值的原始数据字符串
     * @return SHA-512哈希值的字符串表示
     */
    std::string StringEncryption::sha512(const std::string &str) {
        // 定义一个数组用于存储哈希结果
        unsigned char hash[SHA512_DIGEST_LENGTH];
        // 创建并初始化一个EVP_MD_CTX对象，用于执行哈希操作
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        // 初始化哈希操作，使用SHA-512 算法
        EVP_DigestInit_ex(ctx, EVP_sha512(), nullptr);
        // 更新哈希操作，将待哈希的字符串传入
        EVP_DigestUpdate(ctx, str.c_str(), str.size());
        // 定义一个变量用于存储哈希结果的长度
        unsigned int len;
        // 完成哈希操作，将结果存储到hash数组中，并更新长度
        EVP_DigestFinal_ex(ctx, hash, &len);
        // 释放EVP_MD_CTX对象
        EVP_MD_CTX_free(ctx);
        // 创建一个string stream对象，用于将哈希结果转换为十六进制字符串
        std::stringstream ss;
        // 遍历hash数组，将每个字节转换为两位十六进制数，并加入到字符串中
        for (const unsigned char i: hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
        // 返回哈希结果的十六进制字符串表示
        return ss.str();
    }

    /**
     * 将字符串转换为Base64编码
     * @param in : 需要进行Base64编码的字符串
     * @return : 返回Base64编码后的字符串
     */
    std::string StringEncryption::base64_encode(const std::string &in) {
        ensure_openssl_initialized();
        // 定义用于内存操作和Base64编码的生物(BIO)对象
        // 定义缓冲区指针，用于直接访问内存中的数据
        BUF_MEM *bptr;

        // 创建并初始化Base64编码的BIO对象
        BIO *b64 = BIO_new(BIO_f_base64());
        // 创建内存类型的BIO对象，用于存储编码前的数据
        BIO *bmem = BIO_new(BIO_s_mem());
        // 将Base64 BIO对象推入到内存BIO对象之上，形成一个管道
        b64 = BIO_push(b64, bmem);
        // 向Base64 BIO对象写入待编码的数据
        BIO_write(b64, in.c_str(), static_cast<int>(in.size()));
        // 刷新BIO对象，确保所有数据都被正确处理
        BIO_flush(b64);
        // 获取编码后的数据内存指针
        BIO_get_mem_ptr(bmem, &bptr);

        // 创建一个字符串，包含编码后的数据
        std::string result(bptr->data, bptr->length);
        // 释放所有BIO对象，以释放资源
        BIO_free_all(b64);

        // 移除字符串中的换行符
        result.erase(std::ranges::remove(result, '\n').begin(), result.end());
        // 返回最终的编码字符串
        return result;
    }

    /**
     * 使用HMAC-SHA256算法计算数据的散列值，并以Base64编码形式返回。
     *
     * @param data 待计算散列值的原始数据字符串。
     * @param key 用于HMAC计算的密钥字符串。
     * @return 返回计算得到的散列值，以Base64编码形式表示。
     */
    std::string StringEncryption::hmac_sha256(const std::string &data, const std::string &key) {
        // 定义变量len用于存储散列后的数据长度。
        unsigned int len;
        // 定义一个足够大的数组hash，用于存储散列后的数据。
        unsigned char hash[EVP_MAX_MD_SIZE];

        // 执行HMAC-SHA256散列计算，并将结果存储在hash数组中，同时通过len返回计算出的散列长度。
        HMAC(EVP_sha256(),
             key.c_str(),
             static_cast<int>(key.length()),
             reinterpret_cast<const unsigned char *>(data.data()),
             data.length(),
             hash,
             &len);

        // 创建一个字符串流对象ss，用于构建最终散列值字符串。
        std::stringstream ss;
        // 遍历散列数据，并将其以16进制格式添加到字符串流中。
        for (unsigned int i = 0; i < len; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        // 将字符串流中的内容转换为Base64编码形式，并作为函数的返回值。
        return base64_encode(ss.str());
    }

    /**
     * @brief 生成JWT令牌
     *
     * @param header JWT的头部信息
     * @param payload JWT的有效载荷
     * @param secret 用于签名的密钥
     * @return 生成的JWT令牌字符串
     *
     * JWT（JSON Web Token）是由三部分组成的字符串：头部（Header）、有效载荷（Payload）和签名（Signature），
     * 这三部分通过点号（.）连接。此函数首先将头部和有效载荷分别进行Base64编码，并通过点号连接起来，
     * 然后使用HMAC SHA-256算法和密钥对连接后的字符串进行签名，最后将签名结果与前两部分通过点号连接，
     * 形成最终的JWT令牌字符串。
     */
    std::string StringEncryption::generate_jwt(const std::string &header,
                                               const std::string &payload,
                                               const std::string &secret) {
        // 将头部和有效载荷分别进行Base64编码，并通过点号连接
        const std::string data = base64_encode(header) + "." + base64_encode(payload);

        // 使用HMAC SHA-256 算法和密钥对连接后的字符串进行签名
        const std::string signature = hmac_sha256(data, secret);

        // 返回最终的JWT令牌字符串
        return data + "." + signature;
    }


    /**
     * @brief 生成指定长度的随机字符串
     * @param length 字符串长度
     * @return 生成的随机字符串
     */
    std::string StringEncryption::generate_random_string(size_t length) {
        // 可选的字符集
        constexpr char   charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        constexpr size_t max_index = sizeof(charset) - 1;

        // 使用随机设备初始化Mersenne Twister引擎
        std::random_device              rd;
        std::mt19937                    gen(rd());
        std::uniform_int_distribution<> dis(0, max_index - 1);

        // 构建随机字符串
        std::string result;
        for (size_t i = 0; i < length; ++i) {
            result += charset[dis(gen)];
        }

        return result;
    }

    /**
     * @brief 生成为jwt提供的密钥，使用6位随机字符加上当前启动的时间戳并转换为sha512
     * @return 生成完毕的密钥
     */
    std::string StringEncryption::generate_secret() {
        const auto current_time  = std::to_string(Date::get_current_timestamp_32());
        const auto random_string = generate_random_string(6);
        return StringEncryption::sha512(random_string + current_time);
    }

    /**
     * @brief Base64解码
     * @param in 需要解码的Base64字符串
     * @return 解码后的字符串
     */
    std::string StringEncryption::base64_decode(const std::string &in) {
        ensure_openssl_initialized();

        const auto  len = in.size();
        std::string out;

        // Allocate space for the decoded data.
        // The maximum length of the decoded data is 3/4 * len.
        out.resize(len * 3 / 4);

        BIO *bio = BIO_new_mem_buf(in.data(), -1);  // Use -1 to indicate the entire buffer.
        BIO *b64 = BIO_new(BIO_f_base64());
        bio      = BIO_push(b64, bio);

        // Set flags to avoid line breaks.
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

        // Read the decoded data from the BIO.
        const int out_len = BIO_read(bio, &out[0], static_cast<int>(out.size()));

        // Resize the output string to the actual size.
        out.resize(out_len);

        // Free the BIO resources.
        BIO_free_all(bio);

        return out;
    }

    /**
     * @brief 解析JWT令牌
     * @param jwt 待解析的JWT令牌
     * @param secret 用于验证签名的密钥
     * @return 返回解析后的头部和有效载荷的JSON对象
     * @throw std::invalid_argument 如果JWT格式不正确或签名验证失败
     */
    std::optional<schema::Jwt> StringEncryption::parse_jwt(const std::string &jwt, const std::string &secret) {
        // 分割JWT字符串
        std::vector<std::string> parts;
        std::stringstream        ss(jwt);
        std::string              item;
        while (std::getline(ss, item, '.')) {
            parts.push_back(item);
        }

        // 检查JWT的部分数量
        if (parts.size() != 3) {
            throw std::invalid_argument("Invalid JWT format");
        }

        // 解码头部和有效载荷

        // std::string header_json = base64_decode(parts[0]);
        // std::string payload_json = base64_decode(parts[1]);
        std::string header_json  = drogon::utils::base64Decode(parts[0]);
        std::string payload_json = drogon::utils::base64Decode(parts[1]);
        std::string signature    = parts[2];

        // 验证签名
        const std::string expected_signature = hmac_sha256(parts[0] + "." + parts[1], secret);
        if (expected_signature != signature) {
            throw std::invalid_argument("Signature verification failed");
        }

        try {
            // 解析JSON
            nlohmann::json header  = nlohmann::json::parse(header_json);
            nlohmann::json payload = nlohmann::json::parse(payload_json);
        }
        catch (const nlohmann::json::parse_error &e) {
            throw std::invalid_argument("JSON parsing error: " + std::string(e.what()));
        }

        return schema::Jwt{
                .header  = nlohmann::json::parse(header_json),
                .payload = nlohmann::json::parse(payload_json),
        };
    }

    std::string StringFormat::all_lower(const std::string &str) {
        std::string result{};
        std::ranges::transform(str, std::back_inserter(result), [](const char c) { return std::tolower(c); });
        return result;
    }

    std::string StringFormat::all_upper(const std::string &str) {
        std::string result{};
        std::ranges::transform(str, std::back_inserter(result), [](const char c) { return std::toupper(c); });
        return result;
    }

    std::string StringFormat::first_lower(const std::string &str) {
        if (str.empty()) {
            return str;
        }
        return std::to_string(std::tolower(str[0])) + str.substr(1);
    }

    std::string StringFormat::first_upper(const std::string &str) {
        if (str.empty()) {
            return str;
        }
        return std::to_string(std::toupper(str[0])) + str.substr(1);
    }
}  // namespace fluffy_utils
