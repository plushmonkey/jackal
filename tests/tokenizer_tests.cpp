#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <Tokenizer.h>

// gtest.lib, gtest_main.lib

using jackal::util::Tokenizer;

TEST(Tokenizer, TokenCount) {
    {
        Tokenizer tokenizer("INARENA:0:1337");
        Tokenizer::size_type num = tokenizer.tokenize(':');

        EXPECT_EQ(num, tokenizer.size());
        EXPECT_EQ(3, num);
    }

    {
        Tokenizer tokenizer("INARENA:0:1337:");
        Tokenizer::size_type num = tokenizer.tokenize(':');

        EXPECT_EQ(num, tokenizer.size());
        EXPECT_EQ(4, num);
    }

    {
        Tokenizer tokenizer("::::");
        Tokenizer::size_type num = tokenizer.tokenize(':');

        EXPECT_EQ(num, tokenizer.size());
        EXPECT_EQ(5, num);
    }

    {
        Tokenizer tokenizer("");
        Tokenizer::size_type num = tokenizer.tokenize(':');

        EXPECT_EQ(0, num);
    }

    {
        Tokenizer tokenizer("INARENA");
        Tokenizer::size_type num = tokenizer.tokenize(':');

        EXPECT_EQ(1, num);
    }

    {
        Tokenizer tokenizer("INARENA:0:1337:");
        Tokenizer::size_type num = tokenizer.tokenize(':', 2);

        EXPECT_EQ(2, num);
    }

    {
        Tokenizer tokenizer("INARENA:0:1337:");
        Tokenizer::size_type num = tokenizer.tokenize(':', 1);

        EXPECT_EQ(1, num);
    }
}

TEST(Tokenizer, References) {
    Tokenizer tokenizer("INARENA:0:1337:");

    tokenizer.tokenize(':');

    // Number of tokens should be 4 for INARENA:0:1337: with delimiter :
    EXPECT_EQ(4, tokenizer.size());

    // First token should be INARENA
    EXPECT_EQ(0, tokenizer[0].compare("INARENA"));

    // Last token should be empty string
    EXPECT_EQ(0, tokenizer[tokenizer.size() - 1].compare(""));
}

TEST(Tokenizer, Iterators) {
    Tokenizer tokenizer("INARENA:0:1337:");

    tokenizer.tokenize(':');

    auto begin = tokenizer.begin();

    // Beginning iterator should be the first token in the list
    EXPECT_EQ(0, begin->compare("INARENA"));

    auto end = tokenizer.end();
    // Last iterator should be empty string
    EXPECT_EQ(0, (end-1)->compare(""));

    // Iterators should be equal to the references
    for (Tokenizer::size_type i = 0; i < tokenizer.size(); i++)
        EXPECT_EQ(0, tokenizer[i].compare(*begin++));
}
