#include<public/xen.h>
grant_handle_t map(domid_t friend,unsigned int entry, void * shared_page,grant_handle_t *handle){
	/*SETUP THE MAPPING OPERATION*/
	gnttab_map_grant_ref_t map_op;
	map_op.host_addr=shared_page;
	map_op.flags=GNTMAP_host_map;
	mpa_op.ref=entry;
	map_op.dom=friend;
	/*PERFORM THE MAP*/
	HYPERVISOR_grant_table_op(GNTTABOP_map_grant_ref,&op,1);
	/*CHECK IF THE CODE WORKED*/
	if(map_op.status!=GNTST_okay){
		return -1;
	}else{
		/*RETURN THE HANDLE*/
		*handle=map_op.handle;
		return 0;
	}
}
