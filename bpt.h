#pragma once
#ifndef BPT_H_
#define BPT_H_
#include <string>
#include <list>

const int ORDER = 11;

struct node
{
	void * pointer[ORDER];//pointers pointing to child nodes or records(if leaf)
	std::string  keys[ORDER-1];
	node * parent;
	bool is_leaf;
	int num_keys;//keep track number of valid keys
	node * next;//used for queue
};
struct record
{
	std::string name;
	std::string info;
};


//output
int cut( int length );
record * find( node * root, std::string key);
node * find_leaf( node * root, std::string key);
int height( node * root );
int path_to_root( node * root, node * child );
void print_tree( node * root );
void enqueue( node * new_node );
node * dequeue( void );

//INSERTION
record* make_record(std::string value, std::list<record> &rlist);
node * make_node( void );
node * make_leaf( void );
int get_left_index(node * parent, node * left);
node * insert_into_leaf( node * leaf, std::string key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, std::string key, record * pointer);
node * insert_into_node(node * root, node * parent,	int left_index, std::string key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent, int left_index, std::string key, node * right);
node * insert_into_parent(node * root, node * left, std::string key, node * right);
node * insert_into_new_root(node * left, std::string key, node * right);
node * start_new_tree(std::string key, record * pointer);
node * insert( node * root, std::string key, std::list<record> &rlist);

//DELETION
void destroy_tree_nodes(node * root);
node * destroy_tree(node * root);
node * re_insert( node * root, std::list<record> &rlist);
node * deletion(node* root, std::list<record> &rlist);

#endif