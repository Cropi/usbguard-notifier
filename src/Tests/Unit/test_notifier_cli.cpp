#include "NotifierCLI.hpp"

#include <catch.hpp>

using namespace usbguardNotifier;

TEST_CASE("Notifier CLI", "[CLI]")
{
    SECTION("Construct empty")
    {
        CLI::map e;
        CLI cli(e);
        REQUIRE(cli.getDb().empty());
        REQUIRE(cli.getIterator() == cli.getDb().cend());
    }
    CLI::map db = {
        { 1, {}},
        { 2, {}},
        { 3, {}},
        { 4, {}},
        { 5, {}},
        { 6, {}},
        { 7, {}},
        { 8, {}},
        { 9, {}}
    };
    CLI cli(db);
    
    SECTION("Construct non-empty")
    {
        REQUIRE(cli.getDb() == db);
        REQUIRE(cli.getIterator() == cli.getDb().cbegin());
    }

    SECTION("Jump")
    {
        cli.jump("2");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.jump("0");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.jump("-1");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.jump("10");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.jump("9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.jump("9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.jump("1");
        REQUIRE(cli.getIterator() == db.find(1));
    }

    SECTION("Next")
    {
        cli.jump("1");
        REQUIRE(cli.getIterator() == cli.getDb().cbegin());
        
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.next("");
        REQUIRE(cli.getIterator() == db.find(9));
    }

    SECTION("Previous")
    {
        cli.jump("9");
        REQUIRE(cli.getIterator() == cli.getDb().find(9));

        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(1));
        cli.previous("");
        REQUIRE(cli.getIterator() == db.find(1));
    }

    SECTION("Execute - jump")
    {
        cli.execute("jump", "2");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("jump", "0");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("jump", "-1");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("jump", "10");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("jump", "9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("jump", "9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("jump", "1");
        REQUIRE(cli.getIterator() == db.find(1));
    }

    SECTION("Execute - next")
    {
        cli.execute("jump", "1");
        REQUIRE(cli.getIterator() == cli.getDb().cbegin());
        
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("next", "");
        REQUIRE(cli.getIterator() == db.find(9));
    }

    SECTION("Execute - previous")
    {
        cli.execute("jump", "9");
        REQUIRE(cli.getIterator() == cli.getDb().find(9));

        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(1));
        cli.execute("previous", "");
        REQUIRE(cli.getIterator() == db.find(1));
    }

    SECTION("Execute short - j")
    {
        cli.execute("j", "2");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("j", "0");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("j", "-1");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("j", "10");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("j", "9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("j", "9");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("j", "1");
        REQUIRE(cli.getIterator() == db.find(1));
    }

    SECTION("Execute short - n")
    {
        cli.execute("j", "1");
        REQUIRE(cli.getIterator() == cli.getDb().cbegin());
        
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(9));
        cli.execute("n", "");
        REQUIRE(cli.getIterator() == db.find(9));
    }

    SECTION("Execute short - p")
    {
        cli.execute("j", "9");
        REQUIRE(cli.getIterator() == cli.getDb().find(9));

        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(8));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(7));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(6));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(5));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(4));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(3));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(2));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(1));
        cli.execute("p", "");
        REQUIRE(cli.getIterator() == db.find(1));
    }
}
