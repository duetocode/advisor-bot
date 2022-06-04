#include <gtest/gtest.h>
#include "csv_reader.h"
#include <string>
#include <vector>

TEST(CSVReaderTest, TestReadFromCSV)
{
    auto actual = CSVReader::readCSV("test_data.csv");
    ASSERT_EQ(actual.size(), 500);

    ASSERT_EQ(actual[0].price, 0.02482205);
    ASSERT_EQ(actual[0].amount, 23.9999428);
}

TEST(CSVReaderTest, TestTokenise)
{
    auto actual = CSVReader::tokenise("1,2,3,4,5", ',');
    ASSERT_EQ(actual.size(), 5);
}
