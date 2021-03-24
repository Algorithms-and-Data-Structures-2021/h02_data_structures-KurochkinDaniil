#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  if (size_ > 0){
      Node * new_node = new Node(e, nullptr);
      tail_ -> next = new_node;
      tail_ = new_node;
      size_++;
  }
  else {
      Node * new_node = new Node(e, nullptr);
      head_ = new_node;
      tail_ = new_node;
      size_++;
  }
}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);

  if (index == size_){
      LinkedList::Add(e);
  }
  else if (index == 0){
      Node * new_node = new Node(e, head_);
      head_ = new_node;
      size_++;
  }
  else {
      Node * prev_node = find_node(index-1);
      Node * new_node = new Node(e, prev_node -> next);
      prev_node -> next = new_node;
  }
}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: используйте функцию find_node(index)
  Node * node = find_node(index);
  node -> data = e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
  // Tip 2: используйте функцию find_node(index)
  if (index == 0){
      Node * new_head = head_ -> next;
      auto temp_data = head_ -> data;
      delete head_;
      size_--;
      head_ = new_head;
      return temp_data;
  }
  Node * prev_node = find_node(index-1);
  Node * rem_node = prev_node -> next;
  prev_node -> next = rem_node -> next;
  if (index == size_ - 1){
      tail_ = prev_node;
  }
  size_--;
  Element temp_data = rem_node -> data;
  delete rem_node;
  return temp_data;
}

void LinkedList::Clear() {
  // Tip 1: люди в черном (MIB) пришли стереть вам память
  Node * node = head_;
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
  Node * rem_node;
  while (node != nullptr){
      rem_node = node;
      node = node -> next;
      delete [] rem_node;
  }
}

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  Node * node = find_node(index);
  return node -> data;
}

int LinkedList::IndexOf(Element e) const {
  int index = 0;
  Node * node = head_;
  while(node != nullptr){
      if (node -> data == e){
          return index;
      }
  node = node -> next;
  }
  return -1;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
  // Tip 1: можете сразу обработать случаи поиска начала и конца списка
  if (index == 0){
      return head_;
  }
  if (index == size_ - 1){
      return tail_;
  }
  int index_ = 0;
  for (Node * cur_node = head_; cur_node != nullptr; cur_node = cur_node -> next){
      if (index_ == index){
        return cur_node;
      }
      index_++;
  }
  return {};
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis
