#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <memory>
#include <vector>

#include <optimizer/programIR/basic_block_ir.hpp>
#include <optimizer/programIR/function_ir.hpp>
#include <optimizer/utils/view/flattened_cfg_view.hpp>

namespace
{
using optimizer::program::BasicBlockIR;
using optimizer::program::BasicBlockIR_sptr;
using optimizer::program::FunctionIR;
using optimizer::program::FunctionIR_sptr;
using optimizer::utils::view::FlattenedCFGView;

template <typename ListT>
std::size_t list_size(const ListT& list)
{
    return static_cast<std::size_t>(std::distance(list.begin(), list.end()));
}

FunctionIR_sptr make_function()
{
    return std::make_shared<FunctionIR>();
}

BasicBlockIR_sptr make_block()
{
    return std::make_shared<BasicBlockIR>();
}

void add_block(const FunctionIR_sptr& function, const BasicBlockIR_sptr& block)
{
    function->acquire_basic_block(block);
}

void set_entry_block(const FunctionIR_sptr& function, const BasicBlockIR_sptr& block)
{
    function->get_entry_basic_block() = block;
}

void connect(const BasicBlockIR_sptr& from, const BasicBlockIR_sptr& to)
{
    from->add_successor(to);
    to->add_predecessor(from);
}

} // namespace

TEST_CASE("FlattenedCFGView starts empty for empty function", "[FlattenedCFGView]")
{
    auto function = make_function();

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.empty());
    REQUIRE(cfg.size() == 0);
    REQUIRE(cfg.blocks().empty());
    REQUIRE(cfg.exit_blocks().empty());
}

TEST_CASE("FlattenedCFGView handles single block function", "[FlattenedCFGView]")
{
    auto function = make_function();
    auto block    = make_block();

    add_block(function, block);
    set_entry_block(function, block);

    FlattenedCFGView cfg{function};

    REQUIRE_FALSE(cfg.empty());
    REQUIRE(cfg.size() == 1);

    REQUIRE(cfg.entry() == 0);

    REQUIRE(cfg.block(0) == block);
    REQUIRE(cfg.blocks() == std::vector<BasicBlockIR_sptr>{block});

    REQUIRE(cfg.predecessors(0).empty());
    REQUIRE(cfg.successors(0).empty());

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{0});
    REQUIRE(cfg.is_exit_block(0));

    REQUIRE(cfg.reachable(0));

    REQUIRE(cfg.index_of(block) == 0);
    REQUIRE(cfg.index_of(block.get()) == 0);
}

TEST_CASE("FlattenedCFGView preserves function basic block order", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto b0 = make_block();
    auto b1 = make_block();
    auto b2 = make_block();

    add_block(function, b0);
    add_block(function, b1);
    add_block(function, b2);
    set_entry_block(function, b0);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);

    REQUIRE(cfg.block(0) == b0);
    REQUIRE(cfg.block(1) == b1);
    REQUIRE(cfg.block(2) == b2);

    REQUIRE(cfg.index_of(b0) == 0);
    REQUIRE(cfg.index_of(b1) == 1);
    REQUIRE(cfg.index_of(b2) == 2);
}

TEST_CASE("FlattenedCFGView builds linear predecessors and successors", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto mid   = make_block();
    auto exit  = make_block();

    add_block(function, entry);
    add_block(function, mid);
    add_block(function, exit);
    set_entry_block(function, entry);

    connect(entry, mid);
    connect(mid, exit);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);
    REQUIRE(cfg.entry() == 0);

    REQUIRE(cfg.predecessors(0).empty());
    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{1});

    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0});
    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{2});

    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{1});
    REQUIRE(cfg.successors(2).empty());

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{2});

    REQUIRE_FALSE(cfg.is_exit_block(0));
    REQUIRE_FALSE(cfg.is_exit_block(1));
    REQUIRE(cfg.is_exit_block(2));

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));
}

TEST_CASE("FlattenedCFGView builds branching CFG edges", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto left  = make_block();
    auto right = make_block();
    auto join  = make_block();

    add_block(function, entry);
    add_block(function, left);
    add_block(function, right);
    add_block(function, join);
    set_entry_block(function, entry);

    connect(entry, left);
    connect(entry, right);
    connect(left, join);
    connect(right, join);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 4);
    REQUIRE(cfg.entry() == 0);

    REQUIRE(cfg.predecessors(0).empty());
    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{1, 2});

    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0});
    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{3});

    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{0});
    REQUIRE(cfg.successors(2) == std::vector<std::size_t>{3});

    REQUIRE(cfg.predecessors(3) == std::vector<std::size_t>{1, 2});
    REQUIRE(cfg.successors(3).empty());

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{3});

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));
    REQUIRE(cfg.reachable(3));
}

TEST_CASE("FlattenedCFGView supports entry block that is not first in function order",
          "[FlattenedCFGView]")
{
    auto function = make_function();

    auto dead_before_entry = make_block();
    auto entry             = make_block();
    auto exit              = make_block();

    add_block(function, dead_before_entry);
    add_block(function, entry);
    add_block(function, exit);
    set_entry_block(function, entry);

    connect(entry, exit);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);
    REQUIRE(cfg.entry() == 1);

    REQUIRE(cfg.index_of(dead_before_entry) == 0);
    REQUIRE(cfg.index_of(entry) == 1);
    REQUIRE(cfg.index_of(exit) == 2);

    REQUIRE_FALSE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));

    REQUIRE(cfg.predecessors(0).empty());
    REQUIRE(cfg.successors(0).empty());

    REQUIRE(cfg.predecessors(1).empty());
    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{2});

    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{1});
    REQUIRE(cfg.successors(2).empty());
}

TEST_CASE("FlattenedCFGView marks unreachable blocks", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto exit  = make_block();
    auto dead  = make_block();

    add_block(function, entry);
    add_block(function, exit);
    add_block(function, dead);
    set_entry_block(function, entry);

    connect(entry, exit);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE_FALSE(cfg.reachable(2));

    REQUIRE(cfg.successors(2).empty());
    REQUIRE(cfg.predecessors(2).empty());

    REQUIRE_FALSE(cfg.is_exit_block(0));
    REQUIRE(cfg.is_exit_block(1));
    REQUIRE(cfg.is_exit_block(2));

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{1, 2});
}

TEST_CASE("FlattenedCFGView handles cycles", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto loop  = make_block();
    auto exit  = make_block();

    add_block(function, entry);
    add_block(function, loop);
    add_block(function, exit);
    set_entry_block(function, entry);

    connect(entry, loop);
    connect(loop, loop);
    connect(loop, exit);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);

    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{1});
    REQUIRE(cfg.predecessors(0).empty());

    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{1, 2});
    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0, 1});

    REQUIRE(cfg.successors(2).empty());
    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{1});

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{2});

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));
}

TEST_CASE("FlattenedCFGView handles multiple exit blocks", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto exit1 = make_block();
    auto exit2 = make_block();

    add_block(function, entry);
    add_block(function, exit1);
    add_block(function, exit2);
    set_entry_block(function, entry);

    connect(entry, exit1);
    connect(entry, exit2);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);

    REQUIRE_FALSE(cfg.is_exit_block(0));
    REQUIRE(cfg.is_exit_block(1));
    REQUIRE(cfg.is_exit_block(2));

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{1, 2});

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));
}

TEST_CASE("FlattenedCFGView exposes raw and shared pointer indexing consistently",
          "[FlattenedCFGView]")
{
    auto function = make_function();

    auto b0 = make_block();
    auto b1 = make_block();

    add_block(function, b0);
    add_block(function, b1);
    set_entry_block(function, b0);

    connect(b0, b1);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.index_of(b0) == cfg.index_of(b0.get()));
    REQUIRE(cfg.index_of(b1) == cfg.index_of(b1.get()));

    REQUIRE(cfg.index_of(b0) == 0);
    REQUIRE(cfg.index_of(b1) == 1);
}

TEST_CASE("FlattenedCFGView handles unreachable component with internal edge", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto exit  = make_block();
    auto dead1 = make_block();
    auto dead2 = make_block();

    add_block(function, entry);
    add_block(function, exit);
    add_block(function, dead1);
    add_block(function, dead2);
    set_entry_block(function, entry);

    connect(entry, exit);
    connect(dead1, dead2);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 4);

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE_FALSE(cfg.reachable(2));
    REQUIRE_FALSE(cfg.reachable(3));

    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{1});
    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0});

    REQUIRE(cfg.successors(2) == std::vector<std::size_t>{3});
    REQUIRE(cfg.predecessors(3) == std::vector<std::size_t>{2});

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{1, 3});
}

TEST_CASE("FlattenedCFGView preserves successor insertion order", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto b1    = make_block();
    auto b2    = make_block();
    auto b3    = make_block();

    add_block(function, entry);
    add_block(function, b1);
    add_block(function, b2);
    add_block(function, b3);
    set_entry_block(function, entry);

    connect(entry, b3);
    connect(entry, b1);
    connect(entry, b2);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 4);

    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{3, 1, 2});
    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0});
    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{0});
    REQUIRE(cfg.predecessors(3) == std::vector<std::size_t>{0});
}

TEST_CASE("FlattenedCFGView preserves predecessor insertion order", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto p1    = make_block();
    auto p2    = make_block();
    auto p3    = make_block();
    auto join  = make_block();

    add_block(function, entry);
    add_block(function, p1);
    add_block(function, p2);
    add_block(function, p3);
    add_block(function, join);
    set_entry_block(function, entry);

    connect(entry, p1);
    connect(entry, p2);
    connect(entry, p3);

    connect(p3, join);
    connect(p1, join);
    connect(p2, join);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 5);

    REQUIRE(cfg.predecessors(4) == std::vector<std::size_t>{3, 1, 2});
    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{4});
    REQUIRE(cfg.successors(2) == std::vector<std::size_t>{4});
    REQUIRE(cfg.successors(3) == std::vector<std::size_t>{4});
}

TEST_CASE("FlattenedCFGView handles self-loop only block as reachable non-exit",
          "[FlattenedCFGView]")
{
    auto function = make_function();

    auto block = make_block();

    add_block(function, block);
    set_entry_block(function, block);

    connect(block, block);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 1);
    REQUIRE(cfg.entry() == 0);

    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{0});
    REQUIRE(cfg.predecessors(0) == std::vector<std::size_t>{0});

    REQUIRE(cfg.reachable(0));

    REQUIRE_FALSE(cfg.is_exit_block(0));
    REQUIRE(cfg.exit_blocks().empty());
}

TEST_CASE("FlattenedCFGView handles unreachable self-loop component", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto exit  = make_block();
    auto dead  = make_block();

    add_block(function, entry);
    add_block(function, exit);
    add_block(function, dead);
    set_entry_block(function, entry);

    connect(entry, exit);
    connect(dead, dead);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 3);

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE_FALSE(cfg.reachable(2));

    REQUIRE(cfg.successors(2) == std::vector<std::size_t>{2});
    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{2});

    REQUIRE_FALSE(cfg.is_exit_block(2));
    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{1});
}

TEST_CASE("FlattenedCFGView handles diamond followed by tail block", "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry = make_block();
    auto left  = make_block();
    auto right = make_block();
    auto join  = make_block();
    auto exit  = make_block();

    add_block(function, entry);
    add_block(function, left);
    add_block(function, right);
    add_block(function, join);
    add_block(function, exit);
    set_entry_block(function, entry);

    connect(entry, left);
    connect(entry, right);
    connect(left, join);
    connect(right, join);
    connect(join, exit);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 5);

    REQUIRE(cfg.successors(0) == std::vector<std::size_t>{1, 2});
    REQUIRE(cfg.predecessors(1) == std::vector<std::size_t>{0});
    REQUIRE(cfg.predecessors(2) == std::vector<std::size_t>{0});

    REQUIRE(cfg.successors(1) == std::vector<std::size_t>{3});
    REQUIRE(cfg.successors(2) == std::vector<std::size_t>{3});
    REQUIRE(cfg.predecessors(3) == std::vector<std::size_t>{1, 2});

    REQUIRE(cfg.successors(3) == std::vector<std::size_t>{4});
    REQUIRE(cfg.predecessors(4) == std::vector<std::size_t>{3});

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{4});

    for (std::size_t i = 0; i < cfg.size(); ++i)
    {
        REQUIRE(cfg.reachable(i));
    }
}

TEST_CASE("FlattenedCFGView handles two reachable exits and one unreachable exit",
          "[FlattenedCFGView]")
{
    auto function = make_function();

    auto entry     = make_block();
    auto exit1     = make_block();
    auto exit2     = make_block();
    auto dead_exit = make_block();

    add_block(function, entry);
    add_block(function, exit1);
    add_block(function, exit2);
    add_block(function, dead_exit);
    set_entry_block(function, entry);

    connect(entry, exit1);
    connect(entry, exit2);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.size() == 4);

    REQUIRE(cfg.exit_blocks() == std::vector<std::size_t>{1, 2, 3});

    REQUIRE(cfg.reachable(0));
    REQUIRE(cfg.reachable(1));
    REQUIRE(cfg.reachable(2));
    REQUIRE_FALSE(cfg.reachable(3));

    REQUIRE(cfg.is_exit_block(1));
    REQUIRE(cfg.is_exit_block(2));
    REQUIRE(cfg.is_exit_block(3));
}

TEST_CASE("FlattenedCFGView blocks accessor exposes same order as block accessor",
          "[FlattenedCFGView]")
{
    auto function = make_function();

    auto b0 = make_block();
    auto b1 = make_block();
    auto b2 = make_block();
    auto b3 = make_block();

    add_block(function, b0);
    add_block(function, b1);
    add_block(function, b2);
    add_block(function, b3);
    set_entry_block(function, b0);

    FlattenedCFGView cfg{function};

    REQUIRE(cfg.blocks().size() == cfg.size());

    for (std::size_t i = 0; i < cfg.size(); ++i)
    {
        REQUIRE(cfg.blocks()[i] == cfg.block(i));
        REQUIRE(cfg.index_of(cfg.block(i)) == i);
        REQUIRE(cfg.index_of(cfg.block(i).get()) == i);
    }
}
