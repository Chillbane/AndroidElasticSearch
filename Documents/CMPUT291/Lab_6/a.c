#include<stdio.h>
#include<string.h>
#include<db.h>
int main(){
/// Create a table
DB *student_db;
db_create(&student_db,NULL,0);
student_db->set_flags(student_db,DB_DUP);
student_db->open(student_db,NULL,"students.db",NULL,DB_BTREE,DB_CREATE,0664);
//////////////////////////////////////////////////////////////////////
//// Insert data
DBT key , data;
char id[6], name[30];
strcpy(id,"1");
strcpy(name,"Mohammed");
memset(&key,0,sizeof(key));
memset(&data,0,sizeof(data));
key.data = id;
key.size = strlen(id)+1;
data.data = name;
data.size = strlen(name)+1;
student_db->put(student_db,NULL,&key,&data,0);
///
strcpy(id,"1");
strcpy(name,"Jim");
memset(&key,0,sizeof(key));
memset(&data,0,sizeof(data));
key.data = id;
key.size = strlen(id)+1;
data.data = name;
data.size = strlen(name)+1;
student_db->put(student_db,NULL,&key,&data,0);
///
strcpy(id,"1");
strcpy(name,"Bob");
memset(&key,0,sizeof(key));
memset(&data,0,sizeof(data));
key.data = id;
key.size = strlen(id)+1;
data.data = name;
data.size = strlen(name)+1;
student_db->put(student_db,NULL,&key,&data,0);
///////////////////////////////////////////////////////////////////
// Removing data
//memset(&key,0,sizeof(key));
//key.data = id;
//key.size = strlen(id)+1;
//student_db->del(student_db,NULL,&key,0);
//////////////////////////////////////////////////////////////////
// Viewing stored data
//DBT data2;
//strcpy(id,"1");
//memset(&key,0,sizeof(key));
//memset(&data2,0,sizeof(data2));
//key.data = id;
//key.size = strlen(id)+1;
//student_db->get(student_db,NULL,&key,&data2,0);
//if(data2.size >0)
//printf("%s\n",(char *) data2.data);
///////////////////////////////////////////////////////////////
/// Closing file 
if(student_db != NULL)
student_db->close(student_db,0);
return 0;
}
