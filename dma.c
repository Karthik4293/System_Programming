#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

pthread_mutex_t global_mem_lock;

struct header_t{
       size_t size;
       unsigned int is_free;
       struct header_t* next;
};


struct header_t *head = NULL, *tail = NULL;

struct header_t* get_free_mem(size_t size){
       //First fit algorithm
       struct header_t* block = head;
       while(block){
            if(block->is_free && block->size >= size) return block;
            block = block->next;
       }
       return NULL;
}

void* malloc(size_t size){
       if(!size) return NULL;

       struct header_t* header;
       void* block;

       pthread_mutex_lock(&global_mem_lock); //Entering the critical section 
       //The code is still not thread safe since the system call brk is a globally accessable resource.
       
       header = get_free_mem(size); 
       //Try to find the free memory from already allocated memory
       
       if(header){
          header->is_free = 0;
          pthread_mutex_unlock(&global_mem_lock);          
	  return (void*)(header + 1);
       }

       size_t total_size = sizeof(struct header_t) + size;
       block = sbrk(total_size);
              
       if(block == (void*)-1){
          pthread_mutex_unlock(&global_mem_lock);
	  return NULL; 
       }

       header = block;
       header->is_free = 0;
       header->size = size;
       header->next = NULL;

       if(!head) head = header;
       if(tail) tail->next = header;
       tail = header;

       pthread_mutex_unlock(&global_mem_lock);
       return (void*) (header + 1);
}



void free(void*  block){

     if(!block) return;
     struct header_t* header = (struct header_t*) block - 1;

     pthread_mutex_lock(&global_mem_lock); // Enter the critical section
     void* program_break = sbrk(0);

/*
  What needs to be done if the block is at the end of the list is programmed in the next section.
In the below line the header->size is replaced with (char*) block since 
the comparison needs to be done from pointer to pointer. And with program_break being a void* on the rvalue, it cannot be dereferenced. 
*/
     if(sizeof(struct header_t) + (char*)block  == program_break){
          if(head == tail){
	     head = tail = NULL;
	  }
	  else{
	     struct header_t* tmp = head;
	     while(tmp){
	         if(tmp->next == tail){
		    tail = tmp;
		    tail->next = NULL;
		 }
                 tmp = tmp->next;
	     }
	  }
	  sbrk(0 - sizeof(struct header_t) - header->size);
          pthread_mutex_unlock(&global_mem_lock);
          return;
     }
// if it is not at the end of the list convert the header attribute as free and hopefull it'll be used when malloc is called again.
     header->is_free = 1;
     pthread_mutex_unlock(&global_mem_lock);
     return;
}
