#include "order_book.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "step.h"
#include "order_book_entry.h"
#include "csv_reader.h"

static StepIterator endIterator{nullptr};

OrderBook::OrderBook() {}

OrderBook::OrderBook(std::string _csvFileName) : csvFileName(_csvFileName)
{
    open(csvFileName);
}

void OrderBook::open(std::string csvFileName)
{
    if (file.is_open())
    {
        file.close();
        buf.clear();
    }
    file.open(csvFileName, std::ios_base::in);
    readNextStep();
}

OrderBook::~OrderBook()
{
    if (file.is_open())
        file.close();
}

OrderBook::OrderBook(const OrderBook &other)
{
    copy(other);
}

void OrderBook::copy(const OrderBook &other)
{
    csvFileName = other.csvFileName;
    if (other.file.is_open())
    {
        file.open(csvFileName, std::ios_base::in);
        file.seekg(other.file.tellg());
    }
    buf = other.buf;
}

OrderBook &OrderBook::operator=(const OrderBook &other)
{
    if (this != &other)
    {
        copy(other);
    }
    return *this;
}

OrderBook::OrderBook(OrderBook &&other)
{
    swap(std::move(other));
}

void OrderBook::swap(OrderBook &&other)
{
    std::swap(csvFileName, other.csvFileName);
    file.open(csvFileName);
    if (other.file.is_open())
        file.seekg(other.file.tellg());

    std::swap(buf, other.buf);
}

OrderBook &OrderBook::operator==(OrderBook &&other)
{
    swap(std::move(other));
    return *this;
}

Step &OrderBook::at(size_t index)
{
    // The data is buffered in memory, so we can just return the reference
    if (index < buf.size())
        return buf.at(index);

    // we do not support forward random access
    if (index > buf.size())
        throw std::out_of_range("out of range");

    // the index is one step past the end of the buffer, we can try to load more data
    if (isProcessed())
        throw std::out_of_range("out of range");

    readNextStep();

    if (index < buf.size())
        return buf[index];
    else
        throw std::out_of_range("out of range");
}

void OrderBook::readNextStep()
{
    // the fstream has already reached the end of the file
    if (isProcessed())
        throw std::out_of_range("out of range");

    // time to load the next step
    std::vector<OrderBookEntry> entries;
    std::string line;
    std::streampos mark;

    // save the mark if this is the first time we are accessing the part
    while (std::getline(file, line))
    {
        // parse the line into OrderBookEntry object
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        OrderBookEntry entry = CSVReader::stringsToOBE(tokens);

        // stop when we reach the next timestamp
        if (!entries.empty() && entries.back().timestamp != entry.timestamp)
        {
            file.seekg(mark);
            break;
        }
        mark = file.tellg();

        entries.push_back(entry);
    }

    // if we reached the end of the file, close the stream
    if (file.eof())
    {
        file.close();
    }

    if (!entries.empty())
    {
        buf.push_back(Step{std::move(entries)});
    }
}

bool OrderBook::isBeyondEnd(size_t index) const
{
    // if we have reached the end of the file,  we can use the size of the buffer to determine if we are beyond the end
    if (isProcessed())
    {
        return index >= buf.size();
    }
    // otherwise, we can assume that we are still no the road.
    else
    {
        return false;
    }
}

StepIterator OrderBook::begin()
{
    return StepIterator(this);
}

const StepIterator &OrderBook::end()
{
    return endIterator;
}
StepIterator::StepIterator() {}

StepIterator::StepIterator(OrderBook *orderBook) : orderBook(orderBook), pos(0) {}

StepIterator::StepIterator(const StepIterator &other)
{
    copy(other);
}

StepIterator::StepIterator(StepIterator &&other)
{
    swap(std::move(other));
}

StepIterator &StepIterator::operator=(const StepIterator &other)
{
    copy(other);
    return *this;
}

StepIterator &StepIterator::operator=(StepIterator &&other)
{
    swap(std::move(other));
    return *this;
}

void StepIterator::swap(StepIterator &&other)
{
    std::swap(orderBook, other.orderBook);
    std::swap(pos, other.pos);
}

void StepIterator::copy(const StepIterator &other)
{
    orderBook = other.orderBook;
    pos = other.pos;
}

Step &StepIterator::operator*()
{
    return orderBook->at(pos);
}

Step *StepIterator::operator->()
{
    return &orderBook->at(pos);
}

StepIterator &StepIterator::operator++()
{
    // Check if we reached the end
    if (orderBook->isBeyondEnd(pos))
        throw std::out_of_range("out of range");

    // increase the pointer
    ++pos;

    return *this;
}

StepIterator StepIterator::operator++(int)
{
    StepIterator tmp(*this);
    ++*this;
    return tmp;
}

StepIterator &StepIterator::operator--()
{
    if (pos == 0)
        throw std::out_of_range("out of range");

    // descrease the pointer
    --pos;

    return *this;
}

StepIterator StepIterator::operator--(int)
{
    StepIterator tmp(*this);
    --*this;
    return tmp;
}

bool StepIterator::equals(const StepIterator &other) const
{
    // the other is the end iterator
    if (other.orderBook == nullptr)
        return orderBook->isBeyondEnd(pos);

    return pos == other.pos;
}

bool operator==(const StepIterator &lhs, const StepIterator &rhs)
{
    return lhs.equals(rhs);
}
bool operator!=(const StepIterator &lhs, const StepIterator &rhs)
{
    return !lhs.equals(rhs);
}

bool OrderBook::isProcessed() const
{
    return !file.is_open() || file.eof();
}

void OrderBook::update()
{
    // return if all data has been loaded
    if (isProcessed())
        return;

    // time to load the next step
    std::vector<OrderBookEntry> entries;
    std::string line;
    std::streampos mark;

    // save the mark if this is the first time we are accessing the part
    while (std::getline(file, line))
    {
        // parse the line into OrderBookEntry object
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        OrderBookEntry entry = CSVReader::stringsToOBE(tokens);

        // stop when we reach the next timestamp
        if (!entries.empty() && entries.back().timestamp != entry.timestamp)
        {
            file.seekg(mark);
            break;
        }
        mark = file.tellg();

        entries.push_back(entry);
    }

    if (!entries.empty())
    {
        buf.push_back(Step{std::move(entries)});
    }
}

std::vector<Step> OrderBook::getPreviousSteps(StepIterator it, size_t maxSteps)
{
    std::vector<Step> steps;

    while (maxSteps > 0)
    {
        steps.push_back(*it);

        // if we reached the beginning of the order book, stop the loop
        if (it.pos == 0)
        {
            break;
        }

        --it;
        --maxSteps;
    }

    return steps;
}