#pragma once

#include "rex.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

struct Token
{
    enum TYPE
    {
        TYPE_OBJECT_START, // {
        TYPE_OBJECT_END,   // }
        TYPE_ARRAY_START,  // [
        TYPE_ARRAY_END,    // ]
        TYPE_COMMA,        // ,
        TYPE_COLON,        // :
        TYPE_NULL,         // null
        TYPE_BOOL,         // true, false
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_STRING,
        TYPE_EOF,
    } type;

    union
    {
        long long int as_int;
        double as_float;
        bool as_bool;
        char* as_str;
    };
};

struct Tokenizer
{
    Content content;
    unsigned int cursor;
};

inline static Token
tokenizer_top(Tokenizer* self)
{
    do {
        auto ptr = self->content.data + self->cursor;

        switch (*ptr)
        {
        case '{':
            return Token{ Token::TYPE_OBJECT_START };
        case '}':
            return Token{ Token::TYPE_OBJECT_END };
        case '[':
            return Token{ Token::TYPE_ARRAY_START };
        case ']':
            return Token{ Token::TYPE_ARRAY_END };
        case ',':
            return Token{ Token::TYPE_COMMA };
        case ':':
            return Token{ Token::TYPE_COLON };
        case 'n':
            assert(ptr[1] == 'u');
            assert(ptr[2] == 'l');
            assert(ptr[3] == 'l');
            return Token{ Token::TYPE_NULL };
        case 't':
            assert(ptr[1] == 'r');
            assert(ptr[2] == 'u');
            assert(ptr[3] == 'e');
            return Token{ Token::TYPE_BOOL, true };
        case 'f':
            assert(ptr[1] == 'a');
            assert(ptr[2] == 'l');
            assert(ptr[3] == 's');
            assert(ptr[4] == 'e');
            return Token{ Token::TYPE_BOOL, false };
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '-':
        {
            long long int ivalue = 0;
            double fvalue = 0.0;
            double multiplier = 10.0;

            if (*ptr != '-')
                ivalue = *ptr - '0';

            int i = 1;
            for (; (ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '.' ; i++)
            {
                if (ptr[i] == '.')
                {
                    multiplier = 1.0 / multiplier;
                    fvalue = (double)ivalue;
                    continue;
                }

                if (multiplier > 1.0)
                {
                    ivalue *= (long long int)multiplier;
                    ivalue += ptr[i] - '0';
                }
                else
                {
                    fvalue += ((ptr[i] - '0') * multiplier);
                    multiplier *= (1.0 / 10.0);
                }
           }

            if (*ptr == '-')
            {
                ivalue *= -1;
                fvalue *= -1;
            }

            if (multiplier > 1.0)
            {
                Token res = { Token::TYPE_INT };
                res.as_int = ivalue;
                return res;
            }
            else
            {
                Token res = { Token::TYPE_FLOAT };
                res.as_float = fvalue;
                return res;
            }
        }
        case '"':
        {
            int length = 0;
            for (int i = 1; ptr[i] != '"'; length++, i++);

            Token res = Token{ Token::TYPE_STRING };
            res.as_str = (char*)malloc(length + 1);
            for (int i = 0; i < length; ++i)
                res.as_str[i] = ptr[i+1];
            res.as_str[length] = '\0';
            return res;
        }
        default:
            self->cursor += 1;
            break;
        }
    } while (self->cursor < self->content.size);

    return Token{ Token::TYPE_EOF };
}

inline static Token
tokenizer_next(Tokenizer* self)
{
    do {
        auto ptr = self->content.data + self->cursor;

        switch (*ptr)
        {
        case '{':
            self->cursor++;
            return Token{ Token::TYPE_OBJECT_START };
        case '}':
            self->cursor++;
            return Token{ Token::TYPE_OBJECT_END };
        case '[':
            self->cursor++;
            return Token{ Token::TYPE_ARRAY_START };
        case ']':
            self->cursor++;
            return Token{ Token::TYPE_ARRAY_END };
        case ',':
            self->cursor++;
            return Token{ Token::TYPE_COMMA };
        case ':':
            self->cursor++;
            return Token{ Token::TYPE_COLON };
        case 'n':
            assert(ptr[1] == 'u');
            assert(ptr[2] == 'l');
            assert(ptr[3] == 'l');
            self->cursor += 4;
            return Token{ Token::TYPE_NULL };
        case 't':
            assert(ptr[1] == 'r');
            assert(ptr[2] == 'u');
            assert(ptr[3] == 'e');
            self->cursor += 4;
            return Token{ Token::TYPE_BOOL, true };
        case 'f':
            assert(ptr[1] == 'a');
            assert(ptr[2] == 'l');
            assert(ptr[3] == 's');
            assert(ptr[4] == 'e');
            self->cursor += 5;
            return Token{ Token::TYPE_BOOL, false };
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '-':
        {
            long long int ivalue = 0;
            double fvalue = 0.0;
            double multiplier = 10.0;

            if (*ptr != '-')
                ivalue = *ptr - '0';

            int i = 1;
            for (; (ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '.' ; i++)
            {
                if (ptr[i] == '.')
                {
                    multiplier = 1.0 / multiplier;
                    fvalue = (double)ivalue;
                    continue;
                }

                if (multiplier > 1.0)
                {
                    ivalue *= (long long int)multiplier;
                    ivalue += ptr[i] - '0';
                }
                else
                {
                    fvalue += ((ptr[i] - '0') * multiplier);
                    multiplier *= (1.0 / 10.0);
                }
           }

            if (*ptr == '-')
            {
                ivalue *= -1;
                fvalue *= -1;
            }

            self->cursor += i;

            if (multiplier > 1.0)
            {
                Token res = { Token::TYPE_INT };
                res.as_int = ivalue;
                return res;
            }
            else
            {
                Token res = { Token::TYPE_FLOAT };
                res.as_float = fvalue;
                return res;
            }
        }
        case '"':
        {
            int length = 0;
            for (int i = 1; ptr[i] != '"'; length++, i++);

            Token res = Token{ Token::TYPE_STRING };
            res.as_str = (char*)malloc(length + 1);
            for (int i = 0; i < length; ++i)
                res.as_str[i] = ptr[i+1];
            res.as_str[length] = '\0';
            self->cursor += (length + 2);
            return res;
        }
        default:
            self->cursor += 1;
            break;
        }
    } while (self->cursor < self->content.size);

    return Token{ Token::TYPE_EOF };
}

struct JSON_Member;

struct JSON_Value
{
    enum Type
    {
        TYPE_INVALID,
        TYPE_NULL,
        TYPE_BOOL,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_STRING,
        TYPE_OBJECT,
        TYPE_ARRAY,
    } type;

    union
    {
        bool as_bool;
        long long int as_int;
        double as_float;
        const char* as_str;
        struct
        {
            int length;
            JSON_Member* members;
        } as_object;
        struct
        {
            int length;
            JSON_Value* values;
        } as_array;
    };
};

struct JSON_Member
{
    const char* name;
    JSON_Value value;
};

struct JSON
{
    JSON_Value root;
};

inline static JSON_Value
json_parse_object(Tokenizer* tkz);

inline static JSON_Value
json_parse_value(Tokenizer* tkz)
{
    JSON_Value value = {};

    auto tkn = tokenizer_top(tkz);
    switch (tkn.type)
    {
    case Token::TYPE_NULL:
        tkn = tokenizer_next(tkz);
        value.type = JSON_Value::TYPE_NULL;
        break;
    case Token::TYPE_BOOL:
        tkn = tokenizer_next(tkz);
        value.type = JSON_Value::TYPE_BOOL;
        value.as_bool = tkn.as_bool;
        break;
    case Token::TYPE_INT:
        tkn = tokenizer_next(tkz);
        value.type = JSON_Value::TYPE_INT;
        value.as_int = tkn.as_int;
        break;
    case Token::TYPE_FLOAT:
        tkn = tokenizer_next(tkz);
        value.type = JSON_Value::TYPE_FLOAT;
        value.as_float = tkn.as_float;
        break;
    case Token::TYPE_STRING:
        tkn = tokenizer_next(tkz);
        value.type = JSON_Value::TYPE_STRING;
        value.as_str = tkn.as_str;
        break;
    case Token::TYPE_OBJECT_START:
        value = json_parse_object(tkz);
        break;
    case Token::TYPE_ARRAY_START:
    {
        value.type = JSON_Value::TYPE_ARRAY;
        value.as_array.values = (JSON_Value*)malloc(sizeof(JSON_Value) * 64);
        tkn = tokenizer_next(tkz);
        do {
            auto old_values = value.as_array.values;
            value.as_array.values = (JSON_Value*)malloc(sizeof(JSON_Value) * (value.as_array.length + 1));
            for (int i = 0; i < value.as_array.length; ++i)
                value.as_array.values[i] = old_values[i];
            free(old_values);

            value.as_array.values[value.as_array.length++] = json_parse_value(tkz);
            assert(value.as_array.length <= 64);
            tkn = tokenizer_next(tkz);
        } while (tkn.type == Token::TYPE_COMMA);
        assert(tkn.type == Token::TYPE_ARRAY_END);
        break;
    }

    default:
        assert(false);
        break;
    }

    return value;
}

inline static JSON_Member
json_parse_member(Tokenizer* tkz)
{
    auto tkn = tokenizer_next(tkz);
    assert(tkn.type == Token::TYPE_STRING);

    JSON_Member member = { tkn.as_str };

    tkn = tokenizer_next(tkz);
    assert(tkn.type == Token::TYPE_COLON);

    member.value = json_parse_value(tkz);

    return member;
}

inline static JSON_Value
json_parse_object(Tokenizer* tkz)
{
    auto tkn = tokenizer_next(tkz);
    assert(tkn.type == Token::TYPE_OBJECT_START);

    JSON_Value object = { JSON_Value::TYPE_OBJECT };

    // handle empty object
    if (tokenizer_top(tkz).type == Token::TYPE_OBJECT_END)
    {
        tokenizer_next(tkz);
        return object;
    }

    do {
        auto old_members = object.as_object.members;
        object.as_object.members = (JSON_Member*)malloc(sizeof(JSON_Member) * (object.as_object.length + 1));
        for (int i = 0; i < object.as_object.length; ++i)
            object.as_object.members[i] = old_members[i];
        free(old_members);

        object.as_object.members[object.as_object.length++] = json_parse_member(tkz);
        tkn = tokenizer_next(tkz);
    } while (tkn.type == Token::TYPE_COMMA);
    assert(tkn.type == Token::TYPE_OBJECT_END);

    return object;
}

inline static JSON
json_parse(Tokenizer* tkz)
{
    JSON json = {};
    json.root = json_parse_object(tkz);
    return json;
}

inline static bool
str_eq(const char* a, const char* b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (*a != *b)
            return false;
        a++;
        b++;
    }

    return true;
}

inline static JSON_Value
json_find(JSON_Value object, const char* member)
{
    for (int i = 0; i < object.as_object.length; ++i)
    {
        if (str_eq(object.as_object.members[i].name, member))
            return object.as_object.members[i].value;
    }
    return {};
}

inline static JSON
gltf_load(Rex* self)
{
    // parse gltf file
    Content gltf_data = self->file_read("../data/girl/scene.gltf");

    Tokenizer tkz = Tokenizer{gltf_data, 0};
    JSON json = json_parse(&tkz);
    self->free(gltf_data.data);

    return json;
}
