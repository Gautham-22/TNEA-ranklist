#include<stdio.h>                                                                                                                                    
#include<stdlib.h>                                                                                                                                   
enum {                                                                                                                                               
    MAX_LENGTH = 30,                                                                                                                             
    MAX_SUBJECTS = 3,                                                                                                                            
    MAX_SUBJECT_LENGTH = 10,                                                                                                                     
    END_OF_DETAILS = -1,                                                                                                                         
    MATHS_CODE = 0,                                                                                                                              
    PHYSICS_CODE = 1,                                                                                                                            
    CHEMISTRY_CODE = 2                                                                                                                           
};                                                                                                                                                   
struct student {                                                                                                                                     
    char *name;                                                                                                                                  
    int roll_no;                                                                                                                                 
    float *marks;                                                                                                                                
    float cutoff;                                                                                                                                
    int age;                                                                                                                                     
    int rank;                                                                                                                                    
};                                                                                                                                                   
void input_student_details(struct student *);                                                                                                        
void input_name(struct student *);                                                                                                                   
void calculate_cutoff(struct student *);                                                                                                             
void delete_duplicates(struct student **,int,char *);                                                                                                
void allot_rank(struct student *,int,char *);                                                                                                        
void swap(int *,int,int);                                                                                                                            
int main()                                                                                                                                           
{                                                                                                                                                    
    struct student *p; // p is a structure pointer used for traversing array of structures / structure pointers                                  
    p = NULL;                                                                                                                                    
    int n, len = 0; // len is for storing number of students without duplicate entries                                                           
    char filename[MAX_LENGTH];                                                                                                                   
    FILE *fp; // fp denotes a file pointer                                                                                                       
    printf("Enter the name of a file that needs to be used as a storage location : ");                                                           
    scanf("%s",filename);                                                                                                                        
    fp = fopen(filename,"w");                                                                                                                    
    if(!fp){                                                                                                                                     
        printf("File open error.\n");                                                                                                        
        return 1;                                                                                                                            
    }                                                                                                                                            
    printf("Enter the total number of students : ");                                                                                             
    scanf("%d",&n);                                                                                                                              
    getchar();                                                                                                                                   
    struct student *students[n];                                                                                                                 
    printf("Enter the details of %d students : \n",n);                                                                                           
    for(int i=0;i<n;i++){                                                                                                                        
        students[i] = (struct student *)malloc(sizeof(struct student));                                                                      
        input_student_details(students[i]);                                                                                                  
        calculate_cutoff(students[i]);                                                                                                       
        fwrite(students[i],sizeof(struct student),1,fp);                                                                                     
    }                                                                                                                                            
    fclose(fp);                                                                                                                                  
    delete_duplicates(students,n,filename);  // deletes duplicates and updates the file                                                          
    struct student new[n+1]; // new array for student details without duplicates                                                                 
    p = &(new[0]);                                                                                                                               
    //-------------------Adding details to new array without duplicates--------------------//                                                    
    fp = fopen(filename,"r");                                                                                                                    
    for(int i=0;i<n;i++){                                                                                                                        
        if(!fread(p,sizeof(struct student),1,fp)){ //adding contents to new array                                                            
            break;                                                                                                                       
        }                                                                                                                                    
        len++; // storing length of new array                                                                                                
        p++;                                                                                                                                 
    }                                                                                                                                            
    p->roll_no = END_OF_DETAILS; // to denote end of students                                                                                    
    fclose(fp);                                                                                                                                  
    allot_rank(&(new[0]),len,filename);                                                                                                          
    p = &(new[0]);                                                                                                                               
    //-------------------Updating details in new array after sorting-------------------------//                                                  
    fp = fopen(filename,"r");                                                                                                                    
    for(int i=0;i<n;i++){                                                                                                                        
        if(!fread(p,sizeof(struct student),1,fp)){                                                                                           
            break;                                                                                                                       
        }                                                                                                                                    
        p++;                                                                                                                                 
    }                                                                                                                                            
    p->roll_no = END_OF_DETAILS;                                                                                                                 
    fclose(fp);                                                                                                                                  
    printf("------------------------------------------------------------------------------------\n");                                            
    printf("Rank\tName\tRoll_No.\tMaths\tPhysics\tChemistry\tCutoff\tAge\n");                                                                   
    for(int i=0;i<n;i++){                                                                                                                        
        if(new[i].roll_no == END_OF_DETAILS){                                                                                                
            break;                                                                                                                       
        }                                                                                                                                    
        printf("%d\t%s\t%d\t\t%.1f\t%.1f\t%.1f\t\t%.2f\t%d\n",new[i].rank,new[i].name,new[i].roll_no,(new[i].marks)[0],(new[i].marks)[1],(new[i].marks)[2],new[i].cutoff,new[i].age);                                                                                                             
    }                                                                                                                                            
    printf("------------------------------------------------------------------------------------\n");                                            
    return 0;                                                                                                                                    
}                                                                                                                                                    
void input_name(struct student *stud){                                                                                                               
    char *name, c;                                                                                                                               
    name = (char *)malloc(MAX_LENGTH*sizeof(char));                                                                                              
    int i;                                                                                                                                       
    for(i=0;(c=getchar())!='\n' && i<(MAX_LENGTH-1);i++){                                                                                        
            name[i] = c;                                                                                                                         
    }                                                                                                                                            
    name[i] = '\0';                                                                                                                              
    stud->name = name;                                                                                                                           
}                                                                                                                                                    
void input_student_details(struct student *p){ // p denotes a pointer to a particular student                                                        
    printf("Enter the name of a student : ");                                                                                                    
    input_name(p);                                                                                                                               
    printf("Enter the roll number of %s : ",p->name);                                                                                            
    scanf("%d",&(p->roll_no));                                                                                                                   
    float *marks;                                                                                                                                
    marks = (float *)malloc(MAX_SUBJECTS*sizeof(float));                                                                                         
    char subjects[MAX_SUBJECTS][MAX_SUBJECT_LENGTH] = {"maths","physics","chemistry"};                                                           
    for(int i=0;i<MAX_SUBJECTS;i++){                                                                                                             
        printf("Enter marks scored by %s in %s (out of 100) : ",p->name,subjects[i]);                                                        
        scanf("%f",&(marks[i]));                                                                                                             
    }                                                                                                                                            
    p->marks = marks;                                                                                                                            
    printf("Enter the age of %s : ",p->name);                                                                                                    
    scanf("%d",&(p->age));                                                                                                                       
    getchar();                                                                                                                                   
}                                                                                                                                                    
void calculate_cutoff(struct student *p){                                                                                                            
    float cutoff = 0, maths = ((p->marks)[MATHS_CODE]), phy = ((p->marks)[PHYSICS_CODE]), chem = ((p->marks)[CHEMISTRY_CODE]);                   
    cutoff = (maths + ((phy+chem)/2));                                                                                                           
    p->cutoff = cutoff;                                                                                                                          
}                                                                                                                                                    
void delete_duplicates(struct student **students,int n,char *filename){                                                                              
    FILE *tempfp;                                                                                                                                
    tempfp = fopen("temp.txt","w");                                                                                                              
    for(int i=0;i<n-1;i++){                                                                                                                      
        for(int j=i+1;j<n;j++){                                                                                                              
            if((students[i]->roll_no != 0) && (students[i]->roll_no == students[j]->roll_no)){                                           
                students[j]->roll_no = 0; //roll no. is set to 0 denoting the duplicates                                             
            }                                                                                                                            
        }                                                                                                                                    
    }                                                                                                                                            
    for(int i=0;i<n;i++){                                                                                                                        
        if(students[i]->roll_no != 0){ // avoiding duplicates                                                                                
            fwrite(students[i],sizeof(struct student),1,tempfp);                                                                         
        }                                                                                                                                    
    }                                                                                                                                            
    fclose(tempfp);                                                                                                                              
    remove(filename);                                                                                                                            
    rename("temp.txt",filename); // equivalent to updating existing file without duplicates                                                      
}                                                                                                                                                    
void swap(int *index,int i,int j){  // swapping the values at index positions i and j                                                                
    int temp;                                                                                                                                    
    temp = index[i];                                                                                                                             
    index[i] = index[j];                                                                                                                         
    index[j] = temp;                                                                                                                             
}                                                                                                                                                    
void allot_rank(struct student *students,int n,char *filename){  // n denotes number of students                                                     
    int index[n];                                                                                                                                
    FILE *tempfp;                                                                                                                                
    tempfp = fopen("temp.txt","w");                                                                                                              
    for(int i=0;i<n;i++){                                                                                                                        
            index[i] = i;                                                                                                                        
    }                                                                                                                                            
    // first preference to cutoff, then maths marks, then physics marks, then chemistry marks and then age.
    for(int i=0;i<n-1;i++){                                                                                                                      
        for(int j=i+1;j<n;j++){                                                                                                              
            if(students[index[i]].cutoff < students[index[j]].cutoff){                                                                  
                swap(index,i,j);                                                                                                     
            }else if(students[index[i]].cutoff == students[index[j]].cutoff){                                                           
                if((students[index[i]].marks)[MATHS_CODE] < (students[index[j]].marks)[MATHS_CODE]){                                 
                    swap(index,i,j);                                                                                             
                }else if((students[index[i]].marks)[MATHS_CODE] == (students[index[j]].marks)[MATHS_CODE]){                          
                    if((students[index[i]].marks)[PHYSICS_CODE] < (students[index[j]].marks)[PHYSICS_CODE]){                     
                        swap(index,i,j);                                                                                     
                    }else if((students[index[i]].marks)[PHYSICS_CODE] == (students[index[j]].marks)[PHYSICS_CODE]){              
                        if((students[index[i]].marks)[CHEMISTRY_CODE] < (students[index[j]].marks)[CHEMISTRY_CODE]){         
                            swap(index,i,j);                                                                             
                        }else if((students[index[i]].marks)[CHEMISTRY_CODE] == (students[index[j]].marks)[CHEMISTRY_CODE]){  
                            if(students[index[i]].age < students[index[j]].age){                                         
                                swap(index,i,j);                                                                     
                            }                                                                                            
                        }                                                                                                    
                    }                                                                                                            
                }                                                                                                                    
            }                                                                                                                           
        }                                                                                                                                    
    }                                                                                                                                            
    for(int i=0;i<n;i++){                                                                                                                        
        students[index[i]].rank = i+1; // rank starts from 1                                                                                 
        fwrite(&(students[index[i]]),sizeof(struct student),1,tempfp);                                                                       
    }                                                                                                                                            
    fclose(tempfp);
    remove(filename);                                                                                                                            
    rename("temp.txt",filename);                                                                                                                 
} 