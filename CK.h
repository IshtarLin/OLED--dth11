#ifndef __CK_H__
#define __CK_H__

void CK_Init();					
void CK_SendByte(char Data);	

void CK_SendInt(int num);		
void CK_SendIntX(int num);		
void CK_SendFloat(float num);	
void CK_SendFloatX(float num);	
void CK_SendStr(char *p);		
void CK_SendStrX(char *p);		
void CK_Read();	
void CK_SendBack();
// void CK_Send(unsigned char CKdata,unsigned int CKdata_len);

#endif
