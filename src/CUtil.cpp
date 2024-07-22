/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/src/CUtil.cpp
 * Description:general method class
 */
#include "head.hpp"

std::string CUtil::PrintTrace()
{
    unw_cursor_t cursor;
    unw_context_t context;
    std::string temp;
    // Init context
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Traverse call stack
    while (unw_step(&cursor) > 0)
    {
        unw_word_t offset, pc;
        char fname[64];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        fname[0] = '\0';
        unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
        temp.append(fname);
        temp.append("() ");
        temp.append("+0x");
        temp.append(std::to_string(offset));
        temp.append("\n");
    }
    return temp;
}

std::vector<std::string> CUtil::split(std::string inputString, char delimiter)
{
    std::stringstream ss(inputString);
    std::string token;
    std::vector<std::string> result;
    result.reserve(3);
    while (std::getline(ss, token, delimiter))
    {
        result.emplace_back(token);
    }

    return result;
}
