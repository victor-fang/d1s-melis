#include <stdio.h>  
#include "g711codec.h"  
  
/* 
 * function: convert PCM audio format to g711 alaw/ulaw.(zqj) 
 *   InAudioData:   PCM data prepared for encoding to g711 alaw/ulaw. 
 *   OutAudioData:  encoded g711 alaw/ulaw. 
 *   DataLen:       PCM data size. 
 *   reserve:       reserved param, no use. 
 */  
  
#if 0
/*alaw*/  
int PCM2G711a( char *InAudioData, char *OutAudioData, int DataLen, int reserve )  
{     
    //check params.  
    if( (NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen) )  
    {  
        printf("Error, empty data or transmit failed, exit !\n");     
        return -1;  
    }  
    
	//printf("DataLen = %d, %s, %d\n", DataLen, __func__, __LINE__);  
//	printf("DataLen = %d, %d\n", DataLen, __LINE__);

    int Retaen = 0;   
    Retaen = g711a_encode( (unsigned char *)OutAudioData, (short*)InAudioData, DataLen/2 );  
    
	//printf("Retaen = %d, %s, %d\n", Retaen, __func__, __LINE__);  
//	printf("Retaen = %d, %d\n", Retaen, __LINE__);

    return Retaen; //index successfully encoded data len.  
}  
  
/*ulaw*/  
int PCM2G711u( char *InAudioData, char *OutAudioData, int DataLen, int reserve )  
{     
    //check params.  
    if( (NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen) )  
    {  
        printf("Error, empty data or transmit failed, exit !\n");     
        return -1;  
    }  
    //printf("DataLen = %d, %s, %d\n", DataLen, __func__, __LINE__);  
  
    int Retuen = 0;   
    Retuen = g711u_encode( (unsigned char *)OutAudioData, (short*)InAudioData, DataLen/2 );  
    //printf("Retuen = %d, %s, %d\n", Retuen, __func__, __LINE__);  
  
    return Retuen;   
}  
#endif
  
/* 
 * function: convert g711 alaw audio format to PCM.(zqj) 
 *   InAudioData:   g711 alaw data prepared for encoding to PCM. 
 *   OutAudioData:  encoded PCM audio data. 
 *   DataLen:       g711a data size. 
 *   reserve:       reserved param, no use. 
 */  
#if 0  
/*alaw*/  
int G711a2PCM( char *InAudioData, char *OutAudioData, int DataLen, int reserve )  
{  
    //check param.  
    if( (NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen) )  
    {  
        printf("Error, empty data or transmit failed, exit !\n");     
        return -1;  
    }  
    //printf("DataLen = %d, %s, %d\n", DataLen, __func__, __LINE__);  
  
    int Retade = 0;  
    Retade = g711a_decode( (short*)OutAudioData, (unsigned char *)InAudioData, DataLen );  
    //printf("Retade = %d, %s, %d\n", Retade, __func__, __LINE__);  
  
    return Retade;  //index successfully decoded data len.  
}  
#endif
  
/*ulaw*/  
int G711u2PCM( char *InAudioData, char *OutAudioData, int DataLen, int reserve )  
{  
    //check param.  
    if( (NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen) )  
    {  
        printf("Error, empty data or transmit failed, exit !\n");     
        return -1;  
    }  
    //printf("DataLen = %d, %s, %d\n", DataLen, __func__, __LINE__);  
	// printf("g711u_decodeing\n");
    int Retude = 0;  
    Retude = g711u_decode( (short*)OutAudioData, (unsigned char *)InAudioData, DataLen );  
    //printf("Retude = %d, %s, %d\n", Retude, __func__, __LINE__);  
  
    return Retude;    
}  
