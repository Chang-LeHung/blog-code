/*
 * Author : Pierre-Henri Symoneaux
 */

#include <stdlib.h>
#include <string.h>

//Hashtable element structure
typedef struct hash_elem_t {
	struct hash_elem_t* next; // Next element in case of a collision
	void* data;	// Pointer to the stored element
	char key[]; 	// Key of the stored element
} hash_elem_t;

//Hashtabe structure
typedef struct {
	unsigned int capacity;	// Hashtable capacity (in terms of hashed keys)
	unsigned int e_num;	// Number of element currently stored in the hashtable
	hash_elem_t** table;	// The table containaing elements
} hashtable_t;

//Structure used for iterations
typedef struct {
	hashtable_t* ht; 	// The hashtable on which we iterate
	unsigned int index;	// Current index in the table
	hash_elem_t* elem; 	// Curent element in the list
} hash_elem_it;

// Inititalize hashtable iterator on hashtable 'ht'
#define HT_ITERATOR(ht) {ht, 0, ht->table[0]}

char err_ptr;
void* HT_ERROR = &err_ptr; // Data pointing to HT_ERROR are returned in case of error

/* 	Internal funcion to calculate hash for keys.
	It's based on the DJB algorithm from Daniel J. Bernstein.
	The key must be ended by '\0' character.*/
static unsigned int ht_calc_hash(char* key)
{
	unsigned int h = 5381;
	while(*(key++))
		h = ((h << 5) + h) + (*key);
	return h;
}

/* 	Create a hashtable with capacity 'capacity'
	and return a pointer to it*/
hashtable_t* ht_create(unsigned int capacity)
{
	hashtable_t* hasht = malloc(sizeof(hashtable_t));
	if(!hasht)
		return NULL;
	if((hasht->table = malloc(capacity*sizeof(hash_elem_t*))) == NULL)
	{
		free(hasht->table);
		return NULL;
	}
	hasht->capacity = capacity;
	hasht->e_num = 0;
	unsigned int i;
	for(i = 0; i < capacity; i++)
		hasht->table[i] = NULL;
	return hasht;
}

/* 	Store data in the hashtable. If data with the same key are already stored,
	they are overwritten, and return by the function. Else it return NULL.
	Return HT_ERROR if there are memory alloc error*/
void* ht_put(hashtable_t* hasht, char* key, void* data)
{
	if(data == NULL)
		return NULL;
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];

	while(e != NULL)
	{
		if(!strcmp(e->key, key))
		{
			void* ret = e->data;
			e->data = data;
			return ret;
		}
		e = e->next;
	}

	// Getting here means the key doesn't already exist

	if((e = malloc(sizeof(hash_elem_t)+strlen(key)+1)) == NULL)
		return HT_ERROR;
	strcpy(e->key, key);
	e->data = data;

	// Add the element at the beginning of the linked list
	e->next = hasht->table[h];
	hasht->table[h] = e;
	hasht->e_num ++;

	return NULL;
}

/* Retrieve data from the hashtable */
void* ht_get(hashtable_t* hasht, char* key)
{
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];
	while(e != NULL)
	{
		if(!strcmp(e->key, key))
			return e->data;
		e = e->next;
	}
	return NULL;
}

/* 	Remove data from the hashtable. Return the data removed from the table
	so that we can free memory if needed */
void* ht_remove(hashtable_t* hasht, char* key)
{
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];
	hash_elem_t* prev = NULL;
	while(e != NULL)
	{
		if(!strcmp(e->key, key))
		{
			void* ret = e->data;
			if(prev != NULL)
				prev->next = e->next;
			else
				hasht->table[h] = e->next;
			free(e);
			e = NULL;
			hasht->e_num --;
			return ret;
		}
		prev = e;
		e = e->next;
	}
	return NULL;
}

/* List keys. k should have length equals or greater than the number of keys */
void ht_list_keys(hashtable_t* hasht, char** k, size_t len)
{
	if(len < hasht->e_num)
		return;
	int ki = 0; //Index to the current string in **k
	int i = hasht->capacity;
	while(--i >= 0)
	{
		hash_elem_t* e = hasht->table[i];
		while(e)
		{
			k[ki++] = e->key;
			e = e->next;
		}
	}
}

/* 	List values. v should have length equals or greater 
	than the number of stored elements */
void ht_list_values(hashtable_t* hasht, void** v, size_t len)
{
	if(len < hasht->e_num)
		return;
	int vi = 0; //Index to the current string in **v
	int i = hasht->capacity;
	while(--i >= 0)
	{
		hash_elem_t* e = hasht->table[i];
		while(e)
		{
			v[vi++] = e->data;
			e = e->next;
		}
	}
}

/* Iterate through table's elements. */
hash_elem_t* ht_iterate(hash_elem_it* iterator)
{
	while(iterator->elem == NULL)
	{
		if(iterator->index < iterator->ht->capacity - 1)
		{
			iterator->index++;
			iterator->elem = iterator->ht->table[iterator->index];
		}
		else
			return NULL;
	}
	hash_elem_t* e = iterator->elem;
	if(e)
		iterator->elem = e->next;
	return e;
}

/* Iterate through keys. */
char* ht_iterate_keys(hash_elem_it* iterator)
{
	hash_elem_t* e = ht_iterate(iterator);
	return (e == NULL ? NULL : e->key);
}

/* Iterate through values. */
void* ht_iterate_values(hash_elem_it* iterator)
{
	hash_elem_t* e = ht_iterate(iterator);
	return (e == NULL ? NULL : e->data);
}

/* 	Removes all elements stored in the hashtable.
	if free_data, all stored datas are also freed.*/
void ht_clear(hashtable_t* hasht, int free_data)
{
	hash_elem_it it = HT_ITERATOR(hasht);
	char* k = ht_iterate_keys(&it);
	while(k != NULL)
	{
		free_data ? free(ht_remove(hasht, k)) : ht_remove(hasht, k);
		k = ht_iterate_keys(&it);
	}
}

/* 	Destroy the hash table, and free memory.
	Data still stored are freed*/
void ht_destroy(hashtable_t* hasht)
{
	ht_clear(hasht, 1); // Delete and free all.
	free(hasht->table);
	free(hasht);
}


#include <stdio.h>
int main()
{
	hashtable_t *ht = ht_create(1024);
	ht_put(ht, "foo", "bar");
	printf("%s\n", (char*)ht_get(ht, "foo"));
	ht_put(ht, "foo", "rab");
	printf("%s\n", (char*)ht_get(ht, "foo"));
	ht_remove(ht, "foo");
	if(!ht_get(ht, "foo"))
		printf("foo removed\n");

	ht_put(ht, "foo", "bar");
	ht_put(ht, "toto", "titi");

	printf("Listing keys\n");
	char* str[ht->e_num];
	unsigned int i;
	ht_list_keys(ht, str, ht->e_num);
	for(i = 0; i < ht->e_num; i++)
		printf("%s\n", str[i]);
	
	printf("Listing values\n");
	ht_list_values(ht, (void**)str, ht->e_num);
	for(i = 0; i < ht->e_num; i++)
		printf("%s\n", str[i]);

	hash_elem_it it = HT_ITERATOR(ht);
	hash_elem_t* e = ht_iterate(&it);
	while(e != NULL)
	{
		printf("%s = %s \n", e->key, (char*)e->data);
		e = ht_iterate(&it);
	}
	
	printf("Iterating keys\n");
	hash_elem_it it2 = HT_ITERATOR(ht);
	char* k = ht_iterate_keys(&it2);
	while(k != NULL)
	{
		printf("%s\n", k);
		k = ht_iterate_keys(&it2);
	}

	ht_destroy(ht);
	return 0;
}