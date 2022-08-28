#pragma once
#ifndef BASE_FILE
#include "Files.h"
#endif
#include <vector>
#ifndef NULL
#define NULL 0
#endif // !NULL
#include <type_traits>
#include <new>


template<class _Tp1, class _Tp2>
inline typename std::enable_if<
	!std::is_same<_Tp1, _Tp2>::value
	&& !std::is_same<_Tp1, _Tp2*>::value, _Tp1>::type
getAsPtr(_Tp2&) {
	return _Tp1();
}

template<class _Tp1, class _Tp2>
inline typename std::enable_if<
	std::is_same<_Tp1, _Tp2>::value, _Tp1>::type
getAsPtr(_Tp2& val) {
	return val;
}

template<class _Tp1, class _Tp2>
inline typename std::enable_if<
	std::is_same<_Tp1, _Tp2*>::value, _Tp1>::type
getAsPtr(_Tp2& val) {
	return &val;
}



template<class _Tp, class _RefTp, class _PtrTp, class RawTp>
class BasicLinkedList
{	
	

	friend class ListNode;
	friend class Iterator;
	friend class RIterator;

protected:
	class Friend {};
	class ListNode;
	class Iterator;
	class RIterator;
public:
	typedef ListNode Node;
	typedef BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp> List;
protected:
	inline static Node emptyNode = Node(false);
	Node* base = NULL;
	Node* endn = NULL;
	Node* current = NULL;
	bool bound = false;

public:
  template<bool tempVal>
  void basicPush(_Tp& val);/* {
		Node* newEnd = isTempVal ? new Node(std::move(val)) : new Node(val);
		if (endn == NULL) {
			base = endn = current = newEnd;
		}
		else {
			endn->next = newEnd;
			newEnd->prev = endn;
			endn = newEnd;

		}
		if (current == NULL) {
			current = endn;
		}
	}*/
  template<>
  void basicPush<true>(_Tp& val) {
    Node* newEnd = new Node(std::move(val));
    if (endn == NULL) {
      base = endn = current = newEnd;
    }
    else {
      endn->next = newEnd;
      newEnd->prev = endn;
      endn = newEnd;

    }
    if (current == NULL) {
      current = endn;
    }
  }
  template<>
  void basicPush<false>(_Tp& val) {
    Node* newEnd = new Node(val);
    if (endn == NULL) {
      base = endn = current = newEnd;
    }
    else {
      endn->next = newEnd;
      newEnd->prev = endn;
      endn = newEnd;

    }
    if (current == NULL) {
      current = endn;
    }
  }
  void push(_Tp& val) {
    basicPush<false>(val);
  }
  void movePush(_Tp& val) {
    basicPush<true>(val);
  }
  
  
	//Iterators
	Iterator begin() {
		reset();
		return Iterator(base, current, (List*)this);
	}
	Node* end() const volatile {
		return endn;
	}
	Iterator cbegin() const {
		reset();
		return Iterator(base, current, this);
	}
	Node* cend() const {
		return endn;
	}

	RIterator rbegin() {
		resetEnd();
		return RIterator(endn, current, this);
	}
	Node* rend() {
		return base;
	}
	RIterator crbegin() const {
		resetEnd();
		return RIterator(endn, current, this);
	}
	Node* crend() const {
		return base;
	}
protected:
	void constructor(List& llist) {
		if (llist.empty()) return;
		for (_Tp l : llist) {
			push(l);
		}
		current = base;

	}
	void constructor(Node* b) {
		if (b == NULL) {
			return;
		}
		base = new Node(b, endn);
		current = base;
	}
	void constructor(_Tp e1, _Tp e2) {
		push_back(e1);
		push_back(e2);
	}
	void constructor(_Tp e) {
		push_back(e);
	}
	void cconstructor(List& list) {
		constructor(list);
	}
	void constructor(const List& list) {
		if (list.empty()) return;
		//base = new Node(list.base, endn);
		auto* l = (char*)&list;
		l--;
		l++;
		List* nl = (List*)l;
		List& al = *nl;
		for (_Tp l : al) {
			push(l);
		}
		current = base;
	}
	void constructor(const volatile List& list) {
		auto* l = (char*)&list;
		l--;
		l++;
		List* nl = (List*)l;
		List& al = *nl;
		for (_Tp l : al) {
			push(l);
		}
		current = base;
	}
	void constructor(volatile List& list) {
		auto* l = (char*)&list;
		l--;
		l++;
		List* nl = (List*)l;
		List& al = *nl;
		for (_Tp l : al) {
			push(l);
		}
		current = base;
	}
	/*void constructor(List&& list) {
		if (list.empty()) return;
		base = new Node(list.base, endn);
		current = base;
	}
	void constructor(const List&& list) {
		if (list.empty()) return;
		base = new Node(list.base, endn);
		current = base;
	}*/
	void constructor(std::initializer_list<_Tp> llist) {
		for (_Tp g : llist) {
			push_back(g);
		}
	}
	/*void constructor(_Tp seed) {
		base = endn = current = new Node(seed);
	}*/
	void destructor() {
		if (empty() || bound);
		else {
			current = base;
			while (!empty()) {
				popBase();
			}
		}
		bound = false;
		base = NULL;
		endn = NULL;
		current = NULL;

	}
public:
	void dissolve() {

		base = endn = current = NULL;
		bound = false;
	}
	static bool notexists(Node& n) {
		return !n.isExistNode;
	}
	static bool exists(Node& n) {
		return !notexists(n);
	}
	bool notexist(Node& n) {
		return !n.isExistNode;
	}
	bool exist(Node& n) {
		return !notexist(n);
	}
	template<typename Arg1, typename Arg2, typename ... Args>
	BasicLinkedList(Arg1 a1, Arg2 a2, Args... args) : BasicLinkedList(_Tp(a1, a2, args...)) {

	}
	BasicLinkedList() {

	}
	BasicLinkedList(_Tp seed) {
		constructor(seed);
	}
	BasicLinkedList(std::initializer_list<_Tp> llist) {
		constructor(llist);
	}
	template<typename ... Args>
	BasicLinkedList(_Tp e1, _Tp e2, Args ... args) : BasicLinkedList(static_cast<_Tp>(args)...) {
		constructor(e1, e2);
	}
	BasicLinkedList(BasicLinkedList& list) {
		constructor(list);
	}
	BasicLinkedList(const BasicLinkedList& llist) {
		constructor(llist);
	}
	BasicLinkedList(volatile BasicLinkedList& list) {
		constructor(list);
	}
	BasicLinkedList(const volatile BasicLinkedList& llist) {
		constructor(llist);
	}
	BasicLinkedList(BasicLinkedList&& llist) noexcept {
		constructor(llist);
		llist.destructor();
	}
	~BasicLinkedList() {
		if (!bound) {
			destructor();
		}
		else {
			int g = 0;
			g++;
		}
	}




	Node& getCurrent() {
		if (current == NULL) {
			return emptyNode;
		}
		return *current;
	}
	Node& getBase() {
		if (base == NULL)
			return emptyNode;
		return *base;
	}
	Node& getEnd() {
		if (endn == NULL) return emptyNode;
		return *endn;
	}
	/*List getCopy() {
		List ret = List();
		ret.cconstructor(*this);
		ret.bound = true;
		return ret;
	}*/
	void addAfter(List& llist, bool moveToEnd = false) {
		List newList = List(llist);
		if (current == NULL) {
			current = base;
		}
		if (newList.empty()) return;
		if (empty()) {
			joinTo(newList);

		}
		else if (current->next == NULL) {
			current->next = newList.base;
			newList.base->prev = current;
			endn = newList.endn;
			if (moveToEnd)
				current = endn;
		}
		else {
			Node* next = current->next;
			next->prev = NULL;
			current->next = NULL;
			current->next = newList.base;
			current->next->prev = current;
			next->prev = newList.endn;
			next->prev->next = next;
			if (moveToEnd)
				current = next;
		}
		newList.dissolve();

	}
	void addAfter(_Tp llist, bool moveToEnd = false) {
		Node* newList = new Node(llist);
		if (current == NULL) {
			current = base;
		}
		if (empty()) {
			base = current = endn = newList;

		}
		else if (current->next == NULL) {
			current->next = newList;
			newList->prev = current;
			endn = newList;
			if (moveToEnd)
				current = endn;
		}
		else {
			Node* next = current->next;
			next->prev = NULL;
			current->next = NULL;
			current->next = newList;
			current->next->prev = current;
			next->prev = newList;
			next->prev->next = next;
			if (moveToEnd)
				current = next;
		}

	}
	void addBefore(List& llist, bool moveToBeg = false) {
		List newList = List(llist);
		if (current == NULL) {
			current = base;
		}
		if (newList.empty()) return;
		if (empty()) {
			joinTo(newList);

		}
		else if (current->prev == NULL) {
			current->prev = newList.endn;
			newList.endn->next = current;
			base = newList.base;
			if (moveToBeg)
				current = base;
		}
		else {
			Node* prev = current->prev;
			prev->next = NULL;
			current->prev = NULL;
			current->prev = newList.endn;
			current->prev->next = current;
			prev->next = newList.base;
			prev->next->prev = prev;
			if (moveToBeg)
				current = prev;
		}
		newList.dissolve();

	}
	void addBefore(_Tp llist, bool moveToBeg = false) {
		Node* newList = new Node(llist);
		if (current == NULL) {
			current = base;
		}
		if (empty()) {
			base = endn = current = newList;

		}
		else if (current->prev == NULL) {
			current->prev = newList;
			newList->next = current;
			base = newList;
			if (moveToBeg)
				current = base;
		}
		else {
			Node* prev = current->prev;
			prev->next = NULL;
			current->prev = NULL;
			current->prev = newList;
			current->prev->next = current;
			prev->next = newList;
			prev->next->prev = prev;
			if (moveToBeg)
				current = prev;
		}

	}
	/*void add(unsigned int amnt) {
		while (amnt-- > 0) {
			add();
		}
	}
	void add() {
		Node* newEnd = new Node();
		if (endn == NULL) {
			base = endn = current = newEnd;
		}
		else {
			endn->next = newEnd;
			newEnd->prev = endn;
			endn = newEnd;

		}
		if (current == NULL) {
			current = endn;
		}
	}*/

	template<typename Arg1, typename Arg2, typename ... Args>
	void push_back(Arg1 a1, Arg2 a2, Args... args) {
		push_back(_Tp(a1, a2, args...));
	}
	void push_back(_Tp& val) {
		push(val);
	}
	template<class r = RawTp>
	inline typename std::enable_if<
		!std::is_same<_Tp, r*&>::value, void>::type
	 push_back(_Tp&& val) {
    movePush(val);

	}
	void push_back(Node& n) {
		push_back(n.val);
	}
	void push_back(List& list) {
		if (list.empty())
			return;
		List addList = List(list);
		if (empty()) {
			joinTo(addList);
		}
		else {
			endn->next = addList.base;
			addList.base->prev = endn;
			endn = addList.endn;
		}
		addList.dissolve();
	}
	template<typename Arg1, typename Arg2, typename ... Args>
	void push_base(Arg1 a1, Arg2 a2, Args... args) {
		push_base(_Tp(a1, a2, args...));
	}
	void push_base(_Tp& val) {
		Node* newBase = new Node(val);
		if (endn == NULL) {
			base = endn = current = newBase;
		}
		else {
			base->prev = newBase;
			newBase->next = base;
			base = newBase;

		}
		if (current == NULL) {
			current = base;
		}
	}
	template<class r = RawTp>
	inline typename std::enable_if<
		!std::is_same<_Tp, r*&>::value, void>::type
	 push_base(_Tp&& val) {
		push_base(val);

	}
	void push_base(Node& n) {
		push_base(n.val);
	}
	void push_base(List& list) {
		if (list.empty())
			return;
		List addList = List(list);
		if (empty()) {
			joinTo(addList);
		}
		else {
			base->prev = addList.endn;
			addList.base->next = base;
			base = addList.base;
		}
		addList.dissolve();
	}
	void popEnd() {
		if (endn == NULL) return;
		if (endn->prev == NULL) {
			delete endn;
			dissolve();
		}
		else {
			Node* oldEnd = endn;
			endn = endn->prev;
			if (current == oldEnd) {
				current = endn;
			}
			oldEnd->dissolve();
			delete oldEnd;
			endn->next = NULL;
		}
	}
	void popBase() {
		if (base == NULL) return;
		if (base->next == NULL) {
			Node* del = base;

			delete del;
			dissolve();
		}
		else {
			Node* oldBase = base;
			base = base->next;
			oldBase->dissolve();
			if (current == oldBase) {
				current = base;
			}
			delete oldBase;
			base->prev = NULL;
		}
	}
#ifndef BASE_FILE
#endif
	void joinTo(List& llist) {
		destructor();
		bound = true;
		endn = llist.endn;
		base = llist.base;
		current = llist.current;
	}
	void popCurrent() {
		if (current == NULL) return;
		if (current->next == NULL && current->prev == NULL) {
			current->dissolve();
			delete current;
			dissolve();
		}
		else if (current == endn) {
			popEnd();
			current = endn;
		}
		else if (current == base) {
			popBase();
			current = base;
		}
		else {
			Node* next = current->next;
			Node* prev = current->prev;
			current->dissolve();
			next->prev = prev;
			prev->next = next;
			delete current;
			current = prev;
		}
	}
	void popCurrentNext() {
		if (current == NULL) return;
		if (current->next == NULL && current->prev == NULL) {
			current->dissolve();
			delete current;
			dissolve();
		}
		else if (current == endn) popEnd();
		else if (current == base) popBase();
		else {
			Node* next = current->next;
			Node* prev = current->prev;
			current->dissolve();
			next->prev = prev;
			prev->next = next;
			delete current;
			current = next;
		}
	}
	/*bool empty() {
		return endn == NULL || base == NULL;
	}*/
	bool empty() const {
		return base == NULL;
	}
	/*bool empty() const& {
		return base == NULL;
	}*/
	void clear() {
		destructor();
		dissolve();
	}
	void setCurrent(Node& n) {
		current = &n;
	}
	void moveCurrentLeft() {
		if (!empty() && current != base) {
			current = current->prev;
		}
	}
	void moveCurrentRight() {
		if (!empty() && current != endn) {
			current = current->next;
		}
	}
	List& operator= (List& llist) {
		destructor();
		constructor(llist);
		bound = false;
		return *this;
	}
	List& operator= (const List& llist) {
		destructor();
		bound = false;
		constructor(llist);
		return *this;
	}
	List& operator= (const volatile List& llist) {
		destructor();
		bound = false;
		constructor(llist);
		bound = false;
		return *this;
	}
	List& operator= (List&& llist) noexcept {
		destructor();
		bound = false;
		constructor(llist);
		bound = false;

		llist.destructor();
		return *this;
	}
private:
	int size(Node* n) {
		if (n->next == NULL) {
			return 0;
		}
		else {
			return size(n->next) + 1;
		}
	}
	bool size(Node* n, int& cSize, int wantSize) {
		if (n->next != NULL) {
			cSize++;
			if (wantSize < cSize) {
				return false;
			}
			return size(n->next, cSize, wantSize);
		}
		else {
			if (wantSize - 1 == cSize) {
				return true;
			}
			return false;
		}
	}
	int iters = 0;
public:
	int size() {
		if (empty()) return 0;
		return size(base) + 1;
	}
	bool size(int wantSize) {
		int cSize = 0;
		if (empty()) {
			if (wantSize == 0) {
				return true;
			}
			return false;
		}
		return size(base, cSize, wantSize);
	}
	bool iter() {

		if (current == NULL) current = base;
		if (empty()) return false;
		if (iters++ == 0) {
			current = base;
			return true;
		}
		else {
			current = current->next;
			if (current == NULL) { // reached end
				iters = 0;
				return false;
			}
			return true;
		}
	}
	void unbind() {
		bound = false;
	}
	Node& operator++() const volatile {
		if (current == endn) {
			return emptyNode;
		}
		else if (current == NULL) {
			char* l = (char*)this;
			List* nl = (List*)l;
			nl->current = base->next;
			return *current;
		}
		else {
			char* l = (char*)this;
			List* nl = (List*)l;
			nl->current = current->next;
			return *current;
		}
	}
	Node& operator--() {
		if (current == base) {
			return emptyNode;
		}
		else if (current == NULL) {

			current = endn->prev;
			return *current;
		}
		else {
			current = current->prev;
			return *current;
		}
	}
private:
	bool comp(Node& l1, Node& l2)  const volatile {
		if (l1.val == l2.val) {
			if (l1.getNext().exists()) {
				if (l2.getNext().exists()) {
					return comp(l1.getNext(), l2.getNext());
				}
				else {
					//If list sizes don't match
					return false;
				}
			}
      else if (!l2.getNext().exists()) {
        return true;
      }
		}
		//if list vals don't match
		return false;
	}
public:

	bool operator==(List& l) {
		if (empty() && l.empty() || &l == (List*)this) {
			return true;
		}
		return comp(getBase(), l.getBase());
	}
  bool operator==(List&& l) {
    return operator==(l);
  }
  bool operator!=(List& l) {
    return !operator==(l);
  }
  bool operator!=(List&& l) {
    return !operator==(l);
  }
	/*operator BasicLinkedList<_Tp&>() {
		BasicLinkedList<_Tp&> ret;
		for (auto& i : *this) {
			ret.push_back(i);
		}
		return ret;
	}*/
	void reset() const volatile {
		//current = base;

		char* l = (char*)this;
		l--;
		l++;
		List* nl = (List*)l;
		nl->current = nl->base;

	}
	void resetEnd() const volatile {
		//A little pointer workaround that allows me to change current
		char* l = (char*)this;
		l--;
		l++;
		List* nl = (List*)l;
		nl->current = nl->endn;
	}

};
template<class _Tp, class _RefTp, class _PtrTp, class RawTp>
//The nodes
class BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp>::ListNode {
	friend class BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp>;
	friend class BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp>::Iterator;
	friend void sdfsdfr();
public:
	typedef ListNode Node;
protected:

	Node* next = NULL;
	Node* prev = NULL;
	//bool deletable = true;
	bool isExistNode = true;
	ListNode(bool isExist) {
		isExistNode = isExist;
	}
	/*void preventDel() {
		deletable = false;
	}*/
public:
	_Tp val = _Tp();
	ListNode(Node* base, Node*& endn) : val(base->val) {
		if (base->next != NULL) {
			next = new Node(base->next, endn);
			next->prev = this;
		}
		else {
			endn = this;
		}
	}
	ListNode(_Tp& val) : val(val) {

	}
  ListNode(_Tp&& val) {
    this->val = std::move(val);
  }
	ListNode(ListNode& n) : val(n.val) {
		isExistNode = n.isExistNode;
		//deletable = n.deletable;
	}
	ListNode(const ListNode& n) : val(n.val) {
		isExistNode = n.isExistNode;
		//deletable = n.deletable;
	}
	~ListNode() {
		
	}

	//void allowDel() {
	//	//deletable = true;
	//}
	void dissolve() {
		next = prev = NULL;
	}

	Node& getNext() const volatile {
		if (next == NULL) {
			return emptyNode;
		}
		return *next;
	}
	Node& getPrev() const volatile {
		if (prev == NULL) {
			return emptyNode;
		}
		return *prev;
	}
	bool exists() const volatile {
		return isExistNode;
	}
	bool notexists() const volatile {
		return !exists();
	}
	bool operator==(Node& n) const volatile {
		return this == &n;
	}




	_PtrTp operator->() {
		return getAsPtr<_PtrTp, _Tp>(val);
	}
	operator _RefTp () {
		return val;
	}
	Node& operator=(_Tp& t) {
		val = t;
		return *this;
	}
	template<class r = RawTp>
	inline typename std::enable_if<
		!std::is_same<_Tp, r*&>::value, Node&>::type
	operator=(_Tp&& t) {
		val = t;
		return *this;
	}
	Node& operator=(Node& t) {
		val = t.val;
		return *this;
	}
};
template<class _Tp, class _RefTp, class _PtrTp, class RawTp>
//The setup for the iterators
class BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp>::Iterator {
protected:
	friend void sdfsdfr();
	typedef ListNode Node;
	typedef BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp> List;
	Node* node;
	Node*& current;
	List* list;
	bool hit = false;
public:
	Iterator(Node* n, Node*& c, List* l) : current(c) {
		list = l;
		node = n;
	}
	Iterator(Node* n, Node*& c, const List* l) : current(c) {
		list = l;
		node = n;
	}
	Iterator(Node* n, Node*& c, volatile List* l) : current(c) {
		list = l;
		node = n;
	}
	Iterator(Node* n, Node*& c, const volatile List* l) : current(c) {
		list = l;
		node = n;
	}
	~Iterator() {

	}
	void operator++() {
		if (list->empty()) {
			hit = true;
			return;
		}
		if (node != current)
			node = current;
		node = node->next;
		if (node != NULL)
			current = node;
		else {
			hit = true;
			list->reset();
		}
	}
	bool operator!=(Node* i) {
		if (list->empty()) return false;
		if (hit) return false;
		if (i == node) {
			if (hit) {
				return false;
			}
			else {
				hit = true;
				return true;
			}
		}
		return i != node;
	}


	//
	_RefTp operator*() {
		return node->val;
	}
};
template<class _Tp, class _RefTp, class _PtrTp, class RawTp>
class BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp>::RIterator {
protected:
	friend void sdfsdfr();
	typedef ListNode Node;
	typedef BasicLinkedList<_Tp, _RefTp, _PtrTp, RawTp> List;
	Node* node;
	Node emptyNode = Node();
	Node*& current;
	List* list;
	bool hit = false;
public:
	RIterator(Node* n, Node*& c, List* l) : current(c) {
		list = l;
		node = n;
	}
	~RIterator() {

	}

	//
	_RefTp operator*() {
		return node->val;
	}
	void operator--() {
		if (node != current)
			node = current;
		node = node->prev;
		if (node != NULL)
			current = node;
		else {
			hit = true;
			list->reset();
		}
	}
	bool operator!=(Node* i) {
		if (list->empty()) return false;
		if (hit) return false;
		if (i == node) {
			return true;
		}
		return i != node;
	}
};



namespace std {
	template<class _Tp>
	class LinkedList : public BasicLinkedList<_Tp, _Tp&, _Tp*, _Tp> {
	public:
		typedef BasicLinkedList<_Tp, _Tp&, _Tp*, _Tp> BaseList;
		template<typename Arg1, typename Arg2, typename ... Args>
		LinkedList(Arg1 a1, Arg2 a2, Args... args) : BaseList(_Tp(a1, a2, args...)) {

		}
		LinkedList() : BaseList() {

		}
		LinkedList(_Tp seed) : BaseList(seed) {

		}
		LinkedList(std::initializer_list<_Tp> llist) : BaseList(llist) {

		}
		template<typename ... Args>
		LinkedList(_Tp e1, _Tp e2, Args ... args) : BaseList(static_cast<_Tp>(args)...) {
			constructor(e1, e2);
		}
		LinkedList(LinkedList& list) : BaseList(list){

		}
		LinkedList(const LinkedList& llist) : BaseList(llist){

		}
		LinkedList(volatile LinkedList& list) : BaseList(list) {

		}
		LinkedList(const volatile LinkedList& llist) : BaseList(llist) {
			
		}
	};
	template<class ptref>
	class LinkedList<ptref*&> : public BasicLinkedList<ptref*&, ptref*&, ptref*, ptref> {
		typedef ptref*& _Tp;
	public:
		typedef BasicLinkedList<ptref*&, ptref*, ptref*, ptref> BaseList;
		template<typename Arg1, typename Arg2, typename ... Args>
		LinkedList(Arg1 a1, Arg2 a2, Args... args) : BaseList(_Tp(a1, a2, args...)) {

		}
		LinkedList() {

		}
		LinkedList(_Tp seed) : BaseList(seed) {

		}
		LinkedList(std::initializer_list<_Tp> llist) : BaseList(llist) {

		}
		template<typename ... Args>
		LinkedList(_Tp e1, _Tp e2, Args ... args) : BaseList(static_cast<_Tp>(args)...) {
			constructor(e1, e2);
		}
		LinkedList(LinkedList& list) : BaseList(list) {

		}
		LinkedList(const LinkedList& llist) : BaseList(llist) {

		}
		LinkedList(volatile LinkedList& list) : BaseList(list) {

		}
		LinkedList(const volatile LinkedList& llist) : BaseList(llist) {

		}
	};
	template<class pt>
	class LinkedList<pt*> : public BasicLinkedList<pt*, pt*, pt*, pt> {
		typedef pt* _Tp;
		typedef BasicLinkedList<pt*, pt*, pt*, pt> BaseList;
	public:
		template<typename Arg1, typename Arg2, typename ... Args>
		LinkedList(Arg1 a1, Arg2 a2, Args... args) : BaseList(_Tp(a1, a2, args...)) {

		}
		LinkedList() : BaseList() {

		}
		LinkedList(_Tp seed) : BaseList(seed) {

		}
		LinkedList(std::initializer_list<_Tp> llist) : BaseList(llist) {

		}
		template<typename ... Args>
		LinkedList(_Tp e1, _Tp e2, Args ... args) : BaseList(static_cast<_Tp>(args)...) {
			constructor(e1, e2);
		}
		LinkedList(LinkedList& list) : BaseList(list) {

		}
		LinkedList(const LinkedList& llist) : BaseList(llist) {

		}
		LinkedList(volatile LinkedList& list) : BaseList(list) {

		}
		LinkedList(const volatile LinkedList& llist) : BaseList(llist) {

		}
	};


}
