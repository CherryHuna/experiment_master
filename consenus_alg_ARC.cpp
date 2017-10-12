# experiment_master

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <algorithm>
#include <math.h>
#define MAXSIZE 600
typedef struct CSNode { 
	int id;
	double value;
	int parentId;
	int isMal; 
	struct CSNode *firstChild;
	struct CSNode *nextSibling;
} CSNode, *CSTree;
typedef struct indata
{
	int id;
	int value;
	int parentId;
}indata;
CSTree csTree;
double newVal[MAXSIZE];//存储节点的新值 
indata in[MAXSIZE];    //存储输入拓扑数据 
double out1[4][MAXSIZE];
void inputdata() 
{
	FILE *fp;
	if(!(fp=fopen("c:\\data.txt","r")))
	{
		printf("error");
		exit(1);
	}
    int index=0;
	while(!feof(fp))
	{
		fscanf(fp,"%d",&in[index].id);
		if(in[index].id!=0)fscanf(fp,"%d%d",&in[index].value,&in[index].parentId);
		index++;		
	}
	fclose(fp);	
}
int q=0;
void createCSTree(CSTree &tree)
{
   
    int c=in[q].id;
    printf("%d ",c);
	if(c ==0)
	{
		tree = NULL;
		q++;
		printf("\n");
	}
	else
	{
		
		printf("%d %d\n",in[q].value,in[q].parentId); 
		tree = (CSTree)malloc(sizeof(CSNode));
		tree->id = c;
		tree->value=in[q].value;
		tree->parentId=in[q].parentId;
		q++;
		createCSTree(tree->firstChild);
		createCSTree(tree->nextSibling);
	}
	
}
void setMal(CSTree tree,int id)
{
	if(tree)
	{
		if(tree->id==id)
		{	
			tree->isMal=1;
			return;
		}
		else
		{ 
			setMal(tree->firstChild,id);
			setMal(tree->nextSibling,id);
		} 
	}
}
int FTotal=0; 
void setMalic()
{
	printf("请输入恶意节点的数目:");
	scanf("%d",&FTotal);
	int id;
	for(int i=0;i<FTotal;i++)
	{
		scanf("%d",&id);
		setMal(csTree,id);
	} 
}

//计算叶子节点的数目，孩子-兄弟二叉树中的叶子节点是指那些没有firstChild的节点
void printTree(CSTree tree)
{
	if(tree)
	{
		if(tree) printf("%d ",tree->parentId);
		printTree(tree->firstChild);
		printTree(tree->nextSibling);
	}
}
void getLeafNum(CSTree tree)
{
	if(tree)
	{
		if(!tree->firstChild) leafNum ++;
		getLeafNum(tree->firstChild);
		getLeafNum(tree->nextSibling);
	}
}
double c=0;
void GetParentVal(CSTree t,int pId,int id1)
{
	if(t)
	{
		if(t->id==pId)
		{
			//printf("%d\n",t->value);
			c=t->value;
			if(t->parentId==id1)c=t->value;

		}
		else
		{
			GetParentVal(t->firstChild, pId,id1);
			GetParentVal(t->nextSibling,pId,id1);
		}
	}
}
double  *getSibValue(CSTree t,int id,int &index,double arr[])
{
	
	if(t)
	{
		if(t->id==id)//a为需要查找的二叉树节点值
		{
		
		    t=t->firstChild;
		    index=0;
		    if(t)
		    {				
				while(t!=NULL)
				{
					//printf("%d\n",t->value);
					arr[index++]=t->value;
					t=t->nextSibling;
				}
			
		    }
		     return arr;
		}
		else
		{
			getSibValue(t->firstChild, id,index,arr);
			getSibValue(t->nextSibling,id,index,arr);
		}
	}
}
double ARC_P(double arrPeerVals[],int n,double ownVal,double parentVal,int FLocal,int isMal)
{
	if(isMal!=1)
	{
	int i;
	arrPeerVals[n]=parentVal;
	arrPeerVals[n+1]=ownVal; 
	std::sort(arrPeerVals,arrPeerVals+n+2);
	int iRangeMin=0;
	for( i=0;i<FLocal;iRangeMin++,i++)
	{
		if(arrPeerVals[iRangeMin]>=ownVal)
		break;
	}
	int iRangeMax=n+1;
	for( i=0;i<FLocal;iRangeMax--,i++)
	{
		if(arrPeerVals[iRangeMax]<=ownVal)
			break;
	}
	double newVal=0;
	for(i=iRangeMin;i<=iRangeMax;i++)
	{
		newVal+=arrPeerVals[i];
	}
	newVal/=(iRangeMax-iRangeMin+1);
    return newVal;	
	}
	else return ownVal;
}
double ARC_Pd(double arrPeerVals[],int n,double ownVal,double parentVal,int FLocal,int isMal)
{
	if(isMal!=1)
	{
	if(n==0)FLocal=0;
	int i;
	arrPeerVals[n]=parentVal;
	arrPeerVals[n+1]=ownVal; 
	std::sort(arrPeerVals,arrPeerVals+n+2);
	int iRangeMin=0;
	for( i=0;i<FLocal;iRangeMin++,i++)
	{
		if(arrPeerVals[iRangeMin]>=ownVal)
		break;
	}
	int iRangeMax=n+1;
	for( i=0;i<FLocal;iRangeMax--,i++)
	{
		if(arrPeerVals[iRangeMax]<=ownVal)
			break;
	}
	double newVal=0;
	for(i=iRangeMin;i<=iRangeMax;i++)
	{
		newVal+=arrPeerVals[i];
	}
	newVal/=(iRangeMax-iRangeMin+1);
    return newVal;	
	}
	else return ownVal;
}
double R_ARC_P(double arrPeerVals[],int n,double ownVal,double parentVal,int FLocal,int isMal)
{
	if(isMal!=1)
	{
	int i;
	arrPeerVals[n]=parentVal;
	arrPeerVals[n+1]=ownVal;
	double RangeMin=std::min(ownVal,parentVal);
	double RangeMax=std::max(ownVal,parentVal);
	int iRangeMin=0; 
	std::sort(arrPeerVals,arrPeerVals+n+2);
	//for(i=0;i<n+2;i++)
		//printf("%d ",arrPeerVals[i]);
	for( i=0;i<FLocal;iRangeMin++,i++)
	{
		if(arrPeerVals[iRangeMin]>=RangeMin)
		break;
	}
	int iRangeMax=n+1;
	for( i=0;i<FLocal;iRangeMax--,i++)
	{
		if(arrPeerVals[iRangeMax]<=RangeMax)
			break;
	}
	double newVal=0;
	for(i=iRangeMin;i<=iRangeMax;i++)
	{
		newVal+=arrPeerVals[i];
	}
	newVal/=(iRangeMax-iRangeMin+1);
    return newVal;	
	}
	else return ownVal;
}
int count1=0;
void reTree(CSTree t2,int FLocal)
{
	if(t2)
	{
		
		GetParentVal(csTree,t2->parentId,t2->id);
		//printf("%d ",c);
		int n;
		double arr[MAXSIZE];
		getSibValue(csTree,t2->id,n,arr);
		//for(int i=0;i<n;i++)
			//printf("%d ",arr[i]);
		//double nVal=R_ARC_P(arr,n,t2->value,c,FLocal,t2->isMal);
		double nVal=ARC_Pd(arr,n,t2->value,c,FLocal,t2->isMal);
		printf("%.1f ",nVal); 
		newVal[count1++]=nVal;
		reTree(t2->firstChild,FLocal);
		reTree(t2->nextSibling,FLocal);
	}
}

int node=0;
void resetVal(CSTree &t,double newval[])
{
	if(t)
	{
		
		t->value=newval[node++];
		resetVal(t->firstChild,newval);
		resetVal(t->nextSibling,newval);
	}
}
int count=0;
void outTxt()
{
	FILE *fp;
	if(!(fp=fopen("c:\\outData.txt","w")))
	{
		printf("erro");
		exit(1);
	}
	int i=0,j;
	int index=0; 
    for(i=0;i<4;i++)
    {
    	while(in[index].value==0)
    	{
    		index++;
    	}
	    fprintf(fp,"%d ",in[index++].value);
    	for(j=0;j<100;j++)
    		fprintf(fp,"%.1f ",out1[i][j]);
   		fprintf(fp,"\n");
    }
	fclose(fp);
}

int main()
{
	
	inputdata();
	createCSTree(csTree);
	setMalic();
    while(count<20)
    {
		reTree(csTree,0); 
		
		resetVal(csTree,newVal);
		printf("\n");
		int i;
		for(i=0;i<node;i++)
		{
			out1[i][count]=newVal[i];
		}	
		count++;
		count1=0;
		node=0;
    }
    outTxt();
   
	return 0;
}
