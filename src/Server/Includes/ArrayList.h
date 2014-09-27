#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_
#include "stdserver.h"

namespace List
{

	template <class T>//template - allows the user to choose what type to store in the list
	class ArrayList
	{
	private://private variables
		int length = 1;//sets the length to 1
		T *a1 = (T*)calloc(length, sizeof(T));//allocates enough memory for 1 item

	public://public functions
		ArrayList()
		{//constructor
			//does nothing but you probably already knew that 

		}
		~ArrayList(){//destructor
			//frees up the memory that a1 is pointing to 
			free(a1);
		}
		inline void add(T item){//adds an item to the list

			if (length != 0)
			{//if length is not equal to 0


				a1 = (T*)realloc(a1, length*sizeof(T));//re-allocates memory 

				a1[length - 1] = item;//adds item to the end of the list
				length++;//adds 1 to the length
			}
			else
			{//else if it is equal to 0


				a1[length] = item;//adds item to the front of the list


			}
		}
		inline void remove(T item){

			if (length != 0)
			{
				length--;//subtracts 1 from the length
				T *b = (T*)calloc(length, sizeof(T));//creates a new pointer to a memory block of size length 	
				int idxModifier = 0;//modifies the index so it adds the items to the array in the right places
				int i = 0;//counter/index
				while (i < length){
					if (a1[i] != item)//if the item at the index of i is not equal to item we want to delete
					{
						b[i] = a1[i + idxModifier];

					}
					else
					{
						idxModifier++;//adds 1 to the index modifier
						b[i] = a1[i + 1];//adds the next item so we dont add the item back to the array


					}

					i++;//adds one to i
				}

				if (count != 0)
					a1 = (T*)realloc(b, length*sizeof(T));
				else
					cout << "The item does not exist!" << endl;//lets the user know that item does not exist

				free(b);//frees up the memory that b allocated at the top of the function
			}
			else
			{
				cout << "You cannot delete anything because there is nothing in the list" << endl;//lets the user know that there is nothing in the list
			}

		}
		inline T get(int i){//gets the integer at the index of i

			
			return length != 0 ? a1[i] : T();//returns a[i] if the length is not 0, if it is 0 then it returns NULL wich is defined as 0
		}
		inline void set(int i, T type)
		{
			a1[i] = type;//sets whatever is at teh index of i to type
		}
		inline int size(){//gets the size
			
			return length != 0 ? length - 1 : NULL;//if the length is not equal to 0 then it returns the length-1 , if it is not 0 then it returns NULL
		}
		inline void print(){//prints the list
			if (length != 0)
			{
				cout << "[";//prints the end bracket
				for (int i = 0; i < size(); i++){

					if (i == 0)//if the index is equal to the beginning of the list
						cout << a1[0];//print the first item
					else
						cout << ", " << a1[i];//prints a comma before the item to seperate the items
				}
				cout << "]" << endl;//prints the end bracket
			}
			else
			{

				cout << "There is nothing in the array to print!" << endl;//lets the user know that the list is empty
			}
		}



	};


};
#endif
