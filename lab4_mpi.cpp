#include "lab4_mpi.h"

#include <malloc.h>
#include <cmath>
#include <algorithm>
#include "mpi.h"

int debug=1;


using namespace std;

void printarrayint(int len, int * arr){
	for(int i=0;i<len;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}
void print2darrayint(int lenx, int leny, int ** arr){
	for(int i=0;i<lenx;i++){
		printarrayint(leny, arr[i]);
	}
}
void witness(int len, char * arr, int ** output){
	(*output)[0]=0;
	int piy;
	if(len%2==0){
		piy=len/2;
	}else{
		piy=len/2+11;
	}
	for(int i=1;i<piy;i++){
		int j;
		for(j=0;j<len;j++){
			if(arr[j]!=arr[j+i]){
				break;
			}
		}
		(*output)[i]=j;
	}

}
int duel(int n, char * text, int lenpchar, char * pchar,int witnessarrlen, int * witnessarr, int i, int j){
	int k = witnessarr[j-i];
	if(j+k>n-1){
		return i;
	}
	if(text[j+k] != pchar[k]){
		return i;
	}
	return j;
}
void np_text_analysis(int n, char * text, int lenpchar, char * pchar,int witnessarrlen, int * witnessarr,int *lenpos, int ** pos){
	int mby2ceil;
	if(lenpchar%2==0){
		mby2ceil=lenpchar/2;

	}else{
		mby2ceil=lenpchar/2+1;
	}
	int maxblocks;
	if(n%mby2ceil==0){
		maxblocks=n/mby2ceil;
	}else{
		maxblocks=n/mby2ceil +1;
	}

	int * blcksize = (int *)malloc(sizeof(int) * (maxblocks));
	int * blcksizesum = (int *)malloc(sizeof(int) * (maxblocks));
	int tempint =n;
	for(int i=0;i<maxblocks;i++){
		blcksize[i]=min(tempint,mby2ceil);
		tempint -= mby2ceil;
		if(i==0){
			blcksizesum[0]==blcksize[0];
		}else{
			blcksizesum[i]=blcksizesum[i-1]+blcksize[i];
		}
	}
	int b = maxblocks-1;
	int * potential_position = (int *)malloc(sizeof(int)*(b+1));

	for(int bi=0;bi<=b;bi++){
		int i;
		if(bi==0){
			i=0;
		}else{
			i = blcksizesum[bi-1];
		}
		int ini_i=i;
		for(int j=ini_i+1;j<= (blcksizesum[bi]-1);j++){
			i = duel(n, text, lenpchar, pchar, witnessarrlen, witnessarr, i, j);

		}
		potential_position[bi]=i;
	}
	int * match_positions_proxy = (int *)malloc(sizeof(int)*(sizeof(int)*(b+1)));
	for(int i=0;i<=b;i++){
		match_positions_proxy[i]=0;
	}
	tempint=0;
	for(int i_index=0;i_index<=b;i_index++){
		int i = potential_position[i_index];
		if(i>n-lenpchar){
			continue;
		}
		bool tempbool=true;
		for(int j=0;j<lenpchar;j++){
			if(pchar[j]!=text[i+j]){
				tempbool=false;
				break;
			}
		}
		if(tempbool==false){
			continue;
		}
		tempint++;
		match_positions_proxy[i_index]=1;

	}
	*lenpos = tempint;
	*pos = (int *)malloc(sizeof(int)*(tempint));
	tempint=0;
	for(int i=0;i<=b;i++){
		if(match_positions_proxy[i]==1){
			(*pos)[tempint]=potential_position[i];
			tempint++;
		}
	}


	// free heap
	free(match_positions_proxy);
	free(potential_position);
	free(blcksizesum);
	free(blcksize);
}

void periodic_pattern_matching_single(int n, char * text, int m, int p, char * pattern, int * nummatches, int ** matchinfo){
	// printf("p is %d\n",p);
	// printf("m is %d\n",m);
	// printf("n is %d\n",n);
	char * pdash = (char *)malloc(sizeof(char) * (2*p-1));
	for(int i=0;i<=2*p-2;i++){
		pdash[i]=pattern[i];
	}
	int * witnessarr = (int *)malloc(sizeof(int)*p);
	witness(2*p-1, pdash, &witnessarr);
	// printf("witness arr is\n");
	// printarrayint(p,witnessarr);
	int lenpos;
	int * pos;
	np_text_analysis(n, text,2*p-1, pdash,p, witnessarr,&lenpos, &pos);
	// printf("algo 1 identified of length %d\n",lenpos);
	// printarrayint(lenpos, pos);
	char * u = (char *)malloc(sizeof(char)*(p));
	for(int i=0;i<=p-1;i++){
		u[i]=pattern[i];
	}
	// printf("u is %s of length %d\n",u, p);
	int k = m/p;

	char * v =(char *)malloc(sizeof(char)*(m - k*p));
	int tempint = k*p;
	for(int i=tempint;i<=m-1;i++){
		v[i-tempint] = pattern[i];
	}
	// printf("v is %s of length %d\n",v, m - k*p);
	char * u2v = (char *)malloc(sizeof(char) * (p*2 + m- k*p));
	for(int i=0;i<p;i++){
		u2v[i]=u[i];
	}
	tempint=p;
	for(int i=0;i<p;i++){
		u2v[i+tempint]=u[i];
	}
	tempint += p;
	for(int i=0;i<m-k*p;i++){
		u2v[i+tempint]=v[i];
	}
	// printf("u2v is %s of length %d\n",u2v, p*2 + m- k*p);
	int * M = (int *)malloc(sizeof(int) * (n));
	for(int i=0;i<=n-1;i++){
		M[i]=0;
		bool tempbool = false;
		for(int j=0;j<lenpos;j++){
			if(pos[j]==i){
				tempbool=true;
				break;
			}
		}
		if(tempbool==false){
			continue;
		}
		//check u2v at i
		tempbool=true;
		for(int j=0;j<p*2 + m- k*p;j++){
			if(u2v[j]!=text[i+j]){
				tempbool=false;
				break;
			}
		}
		if(tempbool==false){
			continue;
		}
		M[i]=1;
	}
	// printf("M is\n");
	// printarrayint(n,M);
	int ** S = (int **)malloc(sizeof(int *)*p);
	int ** C = (int **)malloc(sizeof(int *)*p);

	for(int i=0;i<=p-1;i++){
		int lmax = (n-1-i)/p;
		S[i]=(int *)malloc(sizeof(int) * (lmax +1));
		C[i]=(int *)malloc(sizeof(int) * (lmax +1));
		for(int j=0;j<=lmax;j++){
			S[i][j]=M[i+j*p];
		}
		for(int j=0;j<=lmax;j++){
			C[i][j]=0;
			if(j>lmax-k+2){
				continue;
			}
			bool tempbool=true;
			for(int jinner=0;jinner<k-1;jinner++){
				if(S[i][j+jinner]!=1){
					tempbool=false;
					break;
				}
			}
			if(tempbool==false){
				continue;
			}
			C[i][j]=1;
		}
	}
	int * MATCH = (int *)malloc(sizeof(int)*(n-m+1));
	for(int i=0;i<n-m+1;i++){
		MATCH[i]=0;
	}
	for(int j=0;j<=n-m;j++){
		for(int i=0;i<p;i++){
			if(j>=i && ((j-i)%p)==0){
				int l = ((j-i)/p);
				MATCH[j] = C[i][l];
			}
		}
	}
	tempint=0;
	for(int i=0;i<n-m+1;i++){
		if(MATCH[i]==1){
			tempint++;
		}
	}
	*nummatches=tempint;
	*matchinfo = (int *)malloc(sizeof(int)*tempint);
	tempint=0;
	for(int i=0;i<n-m+1;i++){
		if(MATCH[i]==1){
			(*matchinfo)[tempint] = i;
			tempint++;
		}
	}


	//free heap
	free(MATCH);
	for(int i=0;i<p;i++){
		free(C[i]);
	}
	free(C);
	for(int i=0;i<p;i++){
		free(S[i]);
	}
	free(S);
	free(M);
	free(u2v);
	free(v);
	free(u);
	free(pos);
	free(witnessarr);
	free(pdash);
};


// /*
// 	*****************************************************
// 		TODO -- You must implement this function
// 	*****************************************************
// */
void periodic_pattern_matching (
		int n, 
		char *text, 
		int num_patterns, 
		int *m_set, 
		int *p_set, 
		char **pattern_set, 
		int **match_counts, 
		int **matches)
{
	printf("start\n");
	// printf("text is %s\n",text);
	// printf("patterns are\n");
	// for(int i=0;i<num_patterns;i++){
	// 	printf("%s\n",pattern_set[i]);
	// }
	*match_counts = (int *)malloc(sizeof(int)*num_patterns);
	int ** matches2d = (int **)malloc(sizeof(int *)*num_patterns);
	for(int i=0;i<num_patterns;i++){
		periodic_pattern_matching_single(n, text, m_set[i], p_set[i], pattern_set[i], &((*match_counts)[i]), &(matches2d[i]));
		printf("%d th patter has match count %d and at indexes\n",i,(*match_counts)[i]);
		printarrayint((*match_counts)[i],matches2d[i]);
	}
	int tempint =0;
	for(int i=0;i<num_patterns;i++){
		tempint += (*match_counts)[i];
	}
	*matches= (int *)malloc(sizeof(int) * tempint);
	tempint =0;
	for(int i=0;i<num_patterns;i++){
		int templen = (*match_counts)[i];
		for(int j=0;j<templen;j++){
			(*matches)[tempint+j] = matches2d[i][j];
		}
		tempint+=templen;
	}
	//free heap
	for(int i=0;i<num_patterns;i++){
		free(matches2d[i]);
	}
	free(matches2d);
	// return ;

}
