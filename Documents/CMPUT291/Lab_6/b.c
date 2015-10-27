#include<stdio.h>
#include<string.h>
#include<db.h>
int main(){
/// Create a table
DB *student_db;
DBC* dbcp; 
db_create(&student_db,NULL,0);//NULL
student_db->set_flags(student_db,DB_DUP);
student_db->open(student_db,NULL,"students.db",NULL,DB_BTREE,DB_CREATE,0664);
//////////////////////////////////////////////////////////////////////////
student_db->cursor(student_db,NULL,&dbcp,0);//DB_WRITECURSOR
DBT key,data;
char id[6];
strcpy(id,"1");
memset(&key,0,sizeof(key));
memset(&data,0,sizeof(data));
key.data = id;
key.size = strlen(id)+1;
//printf("%d",data.size);
int ret = dbcp->c_get(dbcp,&key,&data,DB_SET);
while(ret != DB_NOTFOUND)
{
printf("The key is: %s and its data: %s\n",(char*) key.data,(char*) data.data);
ret = dbcp->c_get(dbcp,&key,&data,DB_NEXT_DUP);
}

///////////////////////////////////////////////////////////////
/// Closing file 
dbcp->c_close(dbcp);
if(student_db != NULL)
student_db->close(student_db,0);
return 0;
}
