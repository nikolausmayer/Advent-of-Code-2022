
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <fmt/core.h>

struct Node 
{
  enum class Type {
    File,
    Directory,
  };
  Type m_type = Type::Directory;
  std::size_t m_size = 0;
  std::vector<std::shared_ptr<Node>> m_children;
  std::string m_name;
  Node* m_parent = nullptr;

  std::size_t getSize()
  {
    if (m_type == Type::Directory) {
      if (m_size == 0) {
        std::size_t result = 0;
        for (const auto& child_ptr: m_children) {
          result += child_ptr->getSize();
        }
        m_size = result;
      }
    }
    return m_size;
  }

  std::shared_ptr<Node> getChildWithName(const std::string& name) const
  {
    std::shared_ptr<Node> result;
    for (auto child_ptr: m_children)
      if (child_ptr->m_name == name)
        result = child_ptr;
    return result;
  }

  void print(std::string prefix="") const
  {
    fmt::print("{}{}", prefix, m_name);
    if (m_type == Type::Directory) {
      fmt::print(" (dir)");
    } else {
      fmt::print(" size={}", m_size);
    }
    fmt::print("\n");
    for (const auto& child_ptr: m_children) {
      child_ptr->print(prefix + "  ");
    }
  }
};

int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  Node fs_root{};
  fs_root.m_name = "/";
  Node* current_ptr = &fs_root;

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    if (line[0] == '$') {
      /// Command
      if (line[2] == 'c') {
        /// cd <dir>
        if (line[5] == '.') {
          //fmt::print("go up\n");
          current_ptr = current_ptr->m_parent;
        } else if (line[5] == '/') {
          //fmt::print("(root)\n");
          /// (just to catch the first line)
        } else {
          //fmt::print("cd <{}> -> <{}>\n", current_ptr->m_name, line.substr(5));
          current_ptr = current_ptr->getChildWithName(line.substr(5)).get();
          assert(current_ptr != nullptr);
        }
      }
    } else {
      /// Listing
      auto new_child = std::make_shared<Node>();
      if (line[0] == 'd') {
        /// Directory listing
        new_child->m_name = line.substr(4);
        new_child->m_parent = current_ptr;
        //fmt::print("new directory child <{}>\n", new_child->m_name);
      } else {
        /// File listing
        new_child->m_type = Node::Type::File;
        std::size_t split_idx = 0;
        std::size_t size = 0;
        for (auto c: line) {
          ++split_idx;
          if (c == ' ') {
            break;
          }
          size = size * 10 + (c - '0');
        }
        new_child->m_size = size;
        new_child->m_name = line.substr(split_idx);
        new_child->m_parent = current_ptr;
        //fmt::print("new file child <{}>\n", new_child->m_name);
      }
      current_ptr->m_children.push_back(new_child);
    }
  }
  //fs_root.print();

  std::size_t sum_of_small_sizes = 0;
  std::stack<Node*> stack;
  stack.push(&fs_root);
  while (stack.size()) {
    auto next_ptr = stack.top();
    stack.pop();
    if (auto size = next_ptr->getSize(); size <= 100'000)
      sum_of_small_sizes += size;
    for (auto child_ptr: next_ptr->m_children)
      if (child_ptr->m_type == Node::Type::Directory)
        stack.push(child_ptr.get());
  }
  fmt::print("Sum of small directory sizes: {}\n", sum_of_small_sizes);

  return EXIT_SUCCESS;
}

