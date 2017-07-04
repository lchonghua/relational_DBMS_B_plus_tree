#include <iostream>
#include <string>
#include <algorithm>
#include <list>

#include "bpt.h"

using namespace std;

node * node_queue = NULL;


/* Finds the appropriate place to
 * split a node that is too big into two.
 */
int cut( int length ) 
{
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}



/* Utility function to give the height
 * of the tree, which length in number of edges
 * of the path from the root to any leaf.
 */
int height( node * root ) 
{
	int h = 0;
	node * c = root;
	while (!c->is_leaf) 
	{
		c = (node*) c->pointer[0];
		h++;
	}
	return h;
}


/* Finds and returns the record to which
 * a key refers.
 */
record * find( node * root, string key) 
{
	int i = 0;
	node * c = find_leaf( root, key);
	if (c == NULL) 
		return NULL;
	for (i = 0; i < c->num_keys; i++)
		if (c->keys[i].compare (key) ==0)
			break;
	if (i == c->num_keys) 
		return NULL;
	else
		return (record *) c->pointer[i];
}



/* Traces the path from the root to a leaf, searching by key.
 * Displays information about the path if the verbose flag is set.
  * Returns the leaf containing the given key.
 */
node * find_leaf( node * root, string key) 
{
	int i = 0;
	node * c = root;
	if (c == NULL) 
	{
		//cout<<"\nEmpty tree.";
		return c;
	}
	while (!c->is_leaf) 
	{
		i = 0;
		while (i < c->num_keys) 
		{
			if (key >= c->keys[i]) i++;
			else break;
		}
		c = (node *)c->pointer[i];
	}
	return c;
}


/* Utility function to give the length in edges
 * of the path from any node to the root.
 */
int path_to_root( node * root, node * child ) 
{
	int length = 0;
	node * c = child;
	while (c != root) 
	{
		c = c->parent;
		length++;
	}
	return length;
}



/* Helper function for printing the
 * tree out.  See print_tree.
 */
void enqueue( node * new_node ) 
{
	node * c;
	if (node_queue == NULL) 
	{
		node_queue = new_node;
		node_queue->next = NULL;
	}
	else 
	{
		c = node_queue;
		while(c->next != NULL) 
		{
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}


/* Helper function for printing the
 * tree out.  See print_tree.
 */
node * dequeue( void ) 
{
	node * n = node_queue;
	node_queue = node_queue->next;
	n->next = NULL;
	return n;
}



/* Prints the B+ tree in the command
 * line in level (rank) order, with the 
 * keys in each node and the '|' symbol
 * to separate nodes.
 * With the verbose_output flag set.
 * the values of the pointers corresponding
 * to the keys also appear next to their respective
 * keys, in hexadecimal notation.
 */
void print_tree( node * root ) 
{
	node * n = NULL;
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (root == NULL) 
	{
		printf("Empty tree.\n");
		return;
	}
	node_queue = NULL;
	enqueue(root);
	while( node_queue != NULL ) 
	{
		n = dequeue();
		if (n->parent != NULL && n == n->parent->pointer[0]) 
		{
			new_rank = path_to_root( root, n );
			if (new_rank != rank) 
			{
				rank = new_rank;
				cout<<endl<<endl<<endl;
			}
		}
		cout<<n->keys[0]<<"--- "<<n->keys[n->num_keys-1];
			
		if (!n->is_leaf)
			for (i = 0; i <= n->num_keys; i++)
				enqueue((node*)n->pointer[i]);
		
		cout<<" | ";
	}
	cout<<endl;
}


//INSERTION


/* make a record with the student name 
 * and return a pointer to the record
 */
record* make_record(string value, list<record> &rlist)
{
	
	record temp;
	temp.name = value;
	temp.info = "N/A";
	rlist.push_back(temp);
	list<record>::iterator it = rlist.begin();
	
	while(it != rlist.end())
		it++;
	it--;
	
	return &(*it);
	
}



/* Creates a new general node, which can be adapted
 * to serve as either a leaf or an internal node.
 */
node* make_node(void)
{
	node * new_node;
	new_node = new node;
	for(int i = 0; i < ORDER; i++)
		new_node->pointer[i] = NULL;
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}



/* Creates a new leaf by creating a node
 * and then adapting it appropriately.
 */
node * make_leaf( void ) 
{
	node * leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}



/* Helper function used in insert_into_parent
 * to find the index of the parent's pointer to 
 * the node to the left of the key to be inserted.
 */
int get_left_index(node * parent, node * left) 
{
	int left_index = 0;
	while (left_index <= parent->num_keys && 
			parent->pointer[left_index] != left)
		left_index++;
	return left_index;
}



/* Inserts a new pointer to a record and its 
 * corresponding key into a leaf.
 * Returns the altered leaf.
 */
node * insert_into_leaf( node * leaf, string key, record * pointer ) 
{
	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->num_keys && leaf->keys[insertion_point].compare(key)<0)
		insertion_point++;
	
	for (i = leaf->num_keys; i > insertion_point; i--) 
	{
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pointer[i] = leaf->pointer[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointer[insertion_point] = pointer;
	leaf->num_keys++;
	return leaf;
}



/* Inserts a new key and pointer to a new record into a leaf so as to exceed
 * the tree's order, causing the leaf to be split in half.
 */
node * insert_into_leaf_after_splitting(node * root, node * leaf, string key, record * pointer) 
{
	node * new_leaf;
	string temp_keys[ORDER];//insertion of a key cause the keys to overflow 10->11
	void * temp_pointers[ORDER];
	int insertion_index, split, i, j;
	string  new_key;
	new_leaf = make_leaf();
	
	insertion_index = 0;
	while (insertion_index < ORDER-1 && leaf->keys[insertion_index].compare(key) < 0)
		insertion_index++;

	for (i = 0, j = 0; i < leaf->num_keys; i++, j++) 
	{
		if (j == insertion_index) 
			j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = leaf->pointer[i];
	}

	temp_keys[insertion_index] = key;
	temp_pointers[insertion_index] = pointer;

	leaf->num_keys = 0;

	split = cut(ORDER -1);//Finds the appropriate place to split a node that is too big into two.


	for (i = 0; i < split; i++)
	{
		leaf->pointer[i] = temp_pointers[i];
		leaf->keys[i] = temp_keys[i];
		leaf->num_keys++;
	}

	for (i = split, j = 0; i < ORDER; i++, j++) 
	{
		new_leaf->pointer[j] = temp_pointers[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->num_keys++;
	}
	
	new_leaf->pointer[ORDER - 1] = leaf->pointer[ORDER - 1];
	leaf->pointer[ORDER - 1] = new_leaf;

	for (i = leaf->num_keys; i < ORDER - 1; i++)
		leaf->pointer[i] = NULL;
	for (i = new_leaf->num_keys; i < ORDER - 1; i++)
		new_leaf->pointer[i] = NULL;

	new_leaf->parent = leaf->parent;
	new_key = new_leaf->keys[0];

	return insert_into_parent(root, leaf, new_key, new_leaf);
}



/* Inserts a new key and pointer to a node into a node into 
 * which these can fit without violating the B+ tree properties.
 */
node * insert_into_node(node * root, node * n, int left_index, string key, node * right) 
{
	int i;

	for (i = n->num_keys; i > left_index; i--) 
	{
		n->pointer[i + 1] = n->pointer[i];
		n->keys[i] = n->keys[i - 1];
	}
	n->pointer[left_index + 1] = right;
	n->keys[left_index] = key;
	n->num_keys++;
	return root;
}



/* Inserts a new key and pointer to a node into a node, causing the node's size 
 * to exceed the order, and causing the node to split into two.
 */
node * insert_into_node_after_splitting(node * root, node * old_node, int left_index, 
		string key, node * right) 
{
	int i, j, split;
	string k_prime;
	node * new_node, * child;

	/* First create a temporary set of keys and pointers
	 * to hold everything in order, including
	 * the new key and pointer, inserted in their
	 * correct places. 
	 * Then create a new node and copy half of the 
	 * keys and pointers to the old node and
	 * the other half to the new.
	 */
	string temp_keys[ORDER];
	node * temp_pointers[ORDER+1];

	for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) 
	{
		if (j == left_index + 1) 
			j++;
		temp_pointers[j] = (node*)old_node->pointer[i];//convert void pointer to node type of pointer
	}

	for (i = 0, j = 0; i < old_node->num_keys; i++, j++) 
	{
		if (j == left_index) 
			j++;
		temp_keys[j] = old_node->keys[i];
	}

	temp_pointers[left_index + 1] = right;
	temp_keys[left_index] = key;

	/* Create the new node and copy* half the keys and
	 *  pointers to the old and half to the new.
	 */  
	split = cut(ORDER);
	new_node = make_node();
	old_node->num_keys = 0;
	for (i = 0; i < split - 1; i++) 
	{
		old_node->pointer[i] = temp_pointers[i];
		old_node->keys[i] = temp_keys[i];
		old_node->num_keys++;
	}
	old_node->pointer[i] = temp_pointers[i];
	k_prime = temp_keys[split - 1];
	for (++i, j = 0; i < ORDER; i++, j++) 
	{
		new_node->pointer[j] = temp_pointers[i];
		new_node->keys[j] = temp_keys[i];
		new_node->num_keys++;
	}
	new_node->pointer[j] = temp_pointers[i];
	new_node->parent = old_node->parent;
	for (i = 0; i <= new_node->num_keys; i++) 
	{
		child = (node*) new_node->pointer[i];
		child->parent = new_node;
	}

	/* Insert a new key into the parent of the two
	 * nodes resulting from the split, with
	 * the old node to the left and the new to the right.
	 */
	return insert_into_parent(root, old_node, k_prime, new_node);
}



/* Inserts a new node (leaf or internal node) into the B+ tree.
 * Returns the root of the tree after insertion.
 */
node * insert_into_parent(node * root, node * left, string key, node * right) 
{
	int left_index;
	node * parent;

	parent = left->parent;

	/* Case: new root. */

	if (parent == NULL)
		return insert_into_new_root(left, key, right);

	/* Case: leaf or node. (Remainder of
	 * function body.)  
	 */

	/* Find the parent's pointer to the left 
	 * node.
	 */

	left_index = get_left_index(parent, left);


	/* Simple case: the new key fits into the node. 
	 */

	if (parent->num_keys < ORDER - 1)
		return insert_into_node(root, parent, left_index, key, right);

	/* Harder case:  split a node in order 
	 * to preserve the B+ tree properties.
	 */

	return insert_into_node_after_splitting(root, parent, left_index, key, right);
	//recursive call, finally it will return the root by calling insert_into_node()
}



/* Creates a new root for two subtrees and inserts 
 * the appropriate key into the new root.
 */
node * insert_into_new_root(node * left, string key, node * right) 
{
	node * root = make_node();
	root->keys[0] = key;
	root->pointer[0] = left;
	root->pointer[1] = right;
	root->num_keys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	return root;
}



/* First insertion:
 * start a new tree.
 */
node * start_new_tree(string key, record * pointer) 
{
	node * root = make_leaf();
	root->keys[0] = key;
	root->pointer[0] = pointer;
	root->pointer[ORDER - 1] = NULL;
	root->parent = NULL;
	root->num_keys++;
	return root;
}


/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */
node * insert( node * root, string key, list<record> &rlist) 
{
	record * pointer;
	node * leaf;

	/* The current implementation ignores
	 * duplicates.
	 */

	if (find(root, key) != NULL)
		return root;

	/* Create a new record for the
	 * value.
	 */
	pointer = make_record(key, rlist);


	/* Case: the tree does not exist yet.
	 * Start a new tree.
	 */

	if (root == NULL) 
		return start_new_tree(key, pointer);


	/* Case: the tree already exists.
	 * (Rest of function body.)
	 */

	leaf = find_leaf(root, key);

	/* Case: leaf has room for key and pointer.
	 */

	if (leaf->num_keys < ORDER - 1) 
	{
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}


	/* Case:  leaf must be split.
	 */

	return insert_into_leaf_after_splitting(root, leaf, key, pointer);
}


//DELETION

void destroy_tree_nodes(node * root) 
{
	int i;
	if (root->is_leaf)
		for (i = 0; i < root->num_keys; i++)
			root->pointer[i] = NULL;
	else
		for (i = 0; i < root->num_keys + 1; i++)
			destroy_tree_nodes((node*)root->pointer[i]);
	delete root;
}


node * destroy_tree(node * root) 
{
	destroy_tree_nodes(root);
	return NULL;
}

node * re_insert( node * root, list<record> &rlist) 
{
	record * pointer;
	node * leaf;
		
	for(list<record>::iterator it = rlist.begin(); it != rlist.end(); it++)
	{
		pointer = &(*it);
		
	/* Case: the tree does not exist yet.
	 * Start a new tree.
	 */
		if (root == NULL) 
			root = start_new_tree(it->name, pointer);
		
	/* Case: the tree already exists.
	 * (Rest of function body.)
	 */
		else
		{
			leaf = find_leaf(root, it->name);

		/* Case: leaf has room for key and pointer.
		*/
			if (leaf->num_keys < ORDER - 1) 
			{
				leaf = insert_into_leaf(leaf, it->name, pointer);
			}

		/* Case:  leaf must be split.
		*/
			else
				root = insert_into_leaf_after_splitting(root, leaf, it->name, pointer);
		}
	}
	return root;
}
node * deletion(node* root, list<record> &rlist)
{
	root = destroy_tree(root);
	root = re_insert(root, rlist);
	return root;
}