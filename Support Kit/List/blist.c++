#include "alloc.h"
#include "memory.h"
#include "signals.h"

#include <List.h>
#include <stdio.h>
#include "glue.h"
#include "threads.h"

extern "C" {
	extern sem_id ocaml_sem;
	value b_list_list(value self, value count);
	value b_list_addItem(value list, value item);
	value b_list_countItems(value list);
	value b_list_firstItem(value list);
	value b_list_itemAt(value list, value index);
	value b_list_removeItem(value list, value index);
}

class OList : public BList, public Glue 
{
	public :
		OList(value interne, int32 count):
			BList(count), Glue(interne) {
//			CAMLparam1(interne);
//			CAMLreturn0;
		}
};

//******************
value b_list_list(value self, value count){
	CAMLparam2(self, count);
	CAMLlocal1(list);
	//register_global_root(&list);
	
//	caml_leave_blocking_section();	
	OList *bl;
	list = alloc_small(1,Abstract_tag);

	caml_release_runtime_system();
		bl = new OList(self, Int32_val(count));
	caml_acquire_runtime_system();	
	
	printf("[C] b_list_list 0x%lx : %lx\n", bl, sizeof(OList));fflush(stdout);
	Field(list,0) = (value)bl;
//	caml_enter_blocking_section();
	
	CAMLreturn(list);
}

//*******************
value b_list_addItem(value list, value item){
	CAMLparam2(list, item);
	CAMLlocal1(caml_bool);
	bool res;
	
	OList *bl = (OList *)Field(list,0);
	void *it = (void *)Field(item,0);
	
	printf("[C] b_list_addItem 0x%lx\n",it);fflush(stdout);
//	caml_leave_blocking_section();
	caml_release_runtime_system();
		res = bl->BList::AddItem(it);	
	caml_acquire_runtime_system();
//	caml_enter_blocking_section();
	caml_bool = Val_bool(res);
	
	CAMLreturn(caml_bool);
}

//***********************
value b_list_countItems(value list) {
	CAMLparam1(list);
	CAMLlocal1(caml_count);
	int32 count;
	
	OList *olist = (OList *)Field(list,0);

//	caml_leave_blocking_section();
	caml_release_runtime_system();
		count = olist->BList::CountItems();
	caml_acquire_runtime_system();
//	caml_enter_blocking_section();
	caml_count = caml_copy_int32(count);

	CAMLreturn(caml_count);
}

//***********************
value b_list_firstItem(value list) {
	CAMLparam1(list);
	CAMLlocal1(item);

	OList *olist;
	void *c_item;

	olist = (OList *)Field(list,0);
	
	item = alloc_small(1,Abstract_tag);
	caml_register_global_root(&item);

//	caml_leave_blocking_section();
	caml_release_runtime_system();
		c_item = olist->BList::FirstItem();
	caml_acquire_runtime_system();

	Field(item,0)=(value)c_item;
//	caml_enter_blocking_section();
	
	CAMLreturn(item);
}

//**************************
value b_list_itemAt(value list, value index) {
	CAMLparam2(list, index);
	CAMLlocal1(item);

	int32 ind = Int32_val(index);
	OList *olist;
	void *c_item;

	olist = (OList *)Field(list,0);
	
	item = alloc_small(1,Abstract_tag);
	caml_register_global_root(&item);

//	caml_leave_blocking_section();
	caml_release_runtime_system();
		c_item = olist->BList::ItemAt(ind);
	caml_acquire_runtime_system();

	Field(item,0)=(value)c_item;
//	caml_enter_blocking_section();
	
	CAMLreturn(item);
}

//*************************
value b_list_removeItem(value list, value index) {
	CAMLparam2(list, index);
	CAMLlocal1(item);

	int32 ind = Int32_val(index);
	OList *olist;
	void *c_item;

	olist = (OList *)Field(list,0);
	
	item = alloc_small(1,Abstract_tag);
	caml_register_global_root(&item);

//	caml_leave_blocking_section();
	caml_release_runtime_system();
		c_item = olist->BList::RemoveItem(ind);
	caml_acquire_runtime_system();

	Field(item,0)=(value)c_item;
//	caml_enter_blocking_section();
	
	CAMLreturn(item);
}
