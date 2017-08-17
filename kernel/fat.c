#include"defs.h"

struct FATInfo{
	char	OEMName[8+1];
	ushort	sectorsize;
	uchar	clustersize;
	ushort	fatlocation;
	uchar	fatnum;
	ushort	rootmax;
	ushort	sectornum;
	uchar	mediatype;
	ushort	fatsize;
	ushort	tracksize;
	ushort	headnum;
	uint	hiddensectors; uint	sectornumx;
	uint	fatsizex;
	ushort	fat32extflag;
	ushort	version;
	uint	rootdircluster;
	ushort	fsysinfolocation; ushort	bootcopylocation;
	uchar	driveno;
	uchar	bootsig;
	uint	volumeid;
	char	volumelabel[11+1];
	char	fsystype[8+1];
};

/*
typedef union {
	struct{
		char	Name[11];
		uchar	Attr;
		uchar	NTRes;
		uchar	CrtTimeTenth;
		ushort	FstClusHi;
		ushort	WrtTime;
		ushort	WrtDate;
		uint	FileSize;
	}_attribute_(_packed_)  Short;
	struct{
		uchar	Ord;
		char	Name1[10];
		uchar	Attr;
		uchar	Type;
		uchar	Chksum;
		char	Name2[12];
		ushort	FstClusLO;
		uint	Name3;
	}_attribute_(_packed_)  Long;
}_attribute_(_packed_) directory;
*/


void FATHeadLoad(struct FATInfo* F){
	int A;
	A=0;
	A+=3;
	blockcpy(&F->OEMName,DISK+A,sizeof(char)*8);A+=8;
	F->sectorsize	=	*(ushort*)(DISK+A);A+=2;
	F->clustersize	=	*(ushort*)(DISK+A);A+=1;
	F->fatlocation	=	*(ushort*)(DISK+A);A+=2;
	F->fatnum	=	*(ushort*)(DISK+A);A+=1;
	F->rootmax	=	*(ushort*)(DISK+A);A+=2;
	F->sectornum	=	*(ushort*)(DISK+A);A+=2;
	F->mediatype	=	*(ushort*)(DISK+A);A+=1;
	F->fatsize	=	*(ushort*)(DISK+A);A+=2;
	F->tracksize	=	*(ushort*)(DISK+A);A+=2;
	F->headnum	=	*(ushort*)(DISK+A);A+=2;
	F->hiddensectors=	*(ushort*)(DISK+A);A+=4;
	F->sectornumx	=	*(ushort*)(DISK+A);A+=4;
	F->fatsizex	=	*(ushort*)(DISK+A);A+=4;
	F->fat32extflag	=	*(ushort*)(DISK+A);A+=2;
	F->version	=	*(ushort*)(DISK+A);A+=2;
	F->rootdircluster=	*(ushort*)(DISK+A);A+=4;
	F->fsysinfolocation=	*(ushort*)(DISK+A);A+=2;
	F->bootcopylocation=	*(ushort*)(DISK+A);A+=2; A+=12;
	F->driveno	=	*(ushort*)(DISK+A);A+=1;
	A+=1;
	F->bootsig	=	*(ushort*)(DISK+A);A+=1;
	F->volumeid	=	*(ushort*)(DISK+A);A+=4;
	blockcpy(&F->volumelabel,DISK+A,sizeof(char)*11);A+=11;
	blockcpy(&F->fsystype,DISK+A,sizeof(char)*8);A+=8;
	F->OEMName[8]=0;
	F->volumelabel[11]=0;
	F->fsystype[8]=0;
}

uint* FATLoad(struct FATInfo* fathead){
	int start;
	start=fathead->fatlocation*0x200;
	return ((uint*)(DISK+start));
}

void FATHeadDump(){
	struct FATInfo F;
	FATHeadLoad(&F);

	Printf("%s\n",F.OEMName);
	Printf("%X\n",F.sectorsize);
	Printf("%X\n",F.clustersize);
	Printf("%X\n",F.fatlocation);
	Printf("%X\n",F.fatnum);
	Printf("%X\n",F.rootmax);
	Printf("%X\n",F.sectornum);
	Printf("%X\n",F.mediatype);
	Printf("%X\n",F.fatsize);
	Printf("%X\n",F.tracksize);
	Printf("%X\n",F.headnum);
	Printf("%X\n",F.hiddensectors);
	Printf("%X\n",F.sectornumx);
	Printf("%X\n",F.fatsizex);
	Printf("%X\n",F.fat32extflag);
	Printf("%X\n",F.version);
	Printf("%X\n",F.rootdircluster);
	Printf("%X\n",F.fsysinfolocation);
	Printf("%X\n",F.bootcopylocation);
	Printf("%X\n",F.driveno);
	Printf("%X\n",F.bootsig);
	Printf("%X\n",F.volumeid);
	Printf("%s\n",F.volumelabel);
	Printf("%s\n",F.fsystype);
}

void readcluster(struct FATInfo* fathead,uint cluster,char* buf,int size){
	int start;
	if(size<=0)return;
//		Printf("%X %d %X %d\n",fathead,cluster,buf,size);
	start=(fathead->fatlocation+fathead->fatsizex*fathead->fatnum)*0x200;
	if(size<0x200)		blockcpy(buf,DISK+start+(cluster-2)*0x200,sizeof(char)*size);
	else{
		blockcpy(buf,DISK+start+(cluster-2)*0x200,sizeof(char)*0x200);
	}
	if(0x0FFFFFF8 <= FATLoad(fathead)[cluster])return;
	else	readcluster(fathead,FATLoad(fathead)[cluster],buf+0x200,size-0x200);
}


int allocatedclusternum(struct FATInfo* fathead,uint cluster){
	int A;
	if(0x0FFFFFF8 <= cluster || cluster<=1)return 0;
	return 1+allocatedclusternum(fathead,FATLoad(fathead)[cluster]);
}

void tinyls(){
	int size;
	uchar* rootdir;
	uchar name[12];
	struct FATInfo F;
	int A,B;
	FATHeadLoad(&F);

	size=allocatedclusternum(&F,2);
	rootdir=(char*)mem_alloc(size*0x200);
	readcluster(&F,2,rootdir,size*0x200);

	for(A=0;A*0x20<size*0x200;A++){
		if(rootdir[0x20*A+11]!=0xF && rootdir[0x20*A]!=0 && rootdir[0x20*A]!=0xe5){
			blockcpy(name,&rootdir[0x20*A],sizeof(char)*11);
			for(B=0;B<11;B++)if(name[B]==' ')name[B]=0;
			name[11]=0;
			Printf("%s.%s\t\t%d\n",name,name+8,allocatedclusternum(&F, *((ushort*)(rootdir+0x20*A+0x20-6)) ));
		}
	}
	mem_free(rootdir,size*0x200);
}


file* open(char* filename){
	int size;
	uchar* rootdir;
	file* f;
	uchar name[12];
	uchar buf[12];
	struct FATInfo F;
	int A,B,C;

	FATHeadLoad(&F);

	size=allocatedclusternum(&F,2);
	
	//ルートディレクトリの情報をロード
	rootdir=(char*)mem_alloc(size*0x200);
	readcluster(&F,2,rootdir,size*0x200);

	for(A=0;A*0x20<size*0x200;A++){
		if(rootdir[0x20*A+11]!=0xF && rootdir[0x20*A]!=0 && rootdir[0x20*A]!=0xe5){
			blockcpy(name,&rootdir[0x20*A],sizeof(char)*11);
			for(B=0;B<11;B++)if(name[B]==' ')name[B]=0;
			name[11]=0;
			sformat(buf,"%s.%s",name,name+8);
			if(!strdiff(buf,filename)){
				f=mem_alloc(sizeof(file));
				f->size=*(uint*)(rootdir+0x20*A+0x20-4);
				f->buf=mem_alloc(f->size);
				f->pos=0;
				readcluster(&F,*((ushort*)(rootdir+0x20*A+0x20-6)),f->buf,f->size);
				mem_free(rootdir,size*0x200);
				return f;
			}
		}
	}

	mem_free(rootdir,size*0x200);
	return 0;
}

void read(file* fp,char* buf,int size){
	int A,B;
	if(fp->size >= fp->pos+size){
		blockcpy(buf,fp->buf+fp->pos,size);
		fp->pos+=size;
	}else{
		blockcpy(buf,fp->buf+fp->pos,fp->size-fp->pos);
		buf[fp->size-fp->pos]=0;
		fp->pos=fp->size;
	}
}

int feof(file* fp){
	return fp->pos==fp->size;
}

void close(file* fp){
	mem_free(fp->buf,fp->size);
	mem_free(fp,sizeof(file));
}

void seek(file* fp,int pos){
	fp->pos=pos;
}

void type(char* filename){
	file* fp;
	char buf[65];
	fp=open(filename);
	if(fp==0)return;
	while(!feof(fp)){
		read(fp,buf,64);
		buf[64]=0;
		vputs(buf);
	}
	close(fp);
}

void bintype(char* filename){
	file* fp;
	fp=open(filename);
	bufdump(fp->buf,fp->size);
	close(fp);
}

void FATDump(){
	struct FATInfo F;
	int A;
	FATHeadLoad(&F);
	for(A=0;A<10;A++){
		Printf("0x%X\n",FATLoad(&F)[A]);
	}
}
