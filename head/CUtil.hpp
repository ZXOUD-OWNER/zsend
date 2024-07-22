/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/head/CUtil.hpp
 * Description:general method class
 */
#pragma once
#include "log.hpp"
#include "system.hpp"

struct CUtil
{
    /**
     * @description: Print call stack
     * @return {*} Print call stack as a string
     */
    static std::string PrintTrace();

    /**
     * @description: String Splitting
     * @param {string} content Split String
     * @return {*} Splitting Result
     */
    static std::vector<std::string> split(std::string content, char delimiter);
};
