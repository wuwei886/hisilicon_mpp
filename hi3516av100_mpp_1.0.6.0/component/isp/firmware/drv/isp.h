/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/08/17
  Description   : 
  History       :
  1.Date        : 2010/08/17
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_H__
#define __ISP_H__

#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include "mkp_isp.h"
#include "isp_ext.h"
#include "piris_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define ISP_IRQ_NR              67

typedef struct hiISP_STAT_NODE_S
{
    ISP_STAT_INFO_S stStatInfo;
    struct list_head list;
} ISP_STAT_NODE_S;

typedef struct hiISP_STAT_BUF_S
{
    HI_U32  u32PhyAddr;
    HI_VOID *pVirAddr;
    ISP_STAT_NODE_S astNode[MAX_ISP_STAT_BUF_NUM];
    
    HI_U32 u32UserNum;
    HI_U32 u32BusyNum;
    HI_U32 u32FreeNum;
    struct list_head stUserList;
    struct list_head stBusyList;
    struct list_head stFreeList;
} ISP_STAT_BUF_S;


/*Sync Cfg*/
#define EXP_RATIO_MAX 0xFFF /* max expratio 64X */
#define EXP_RATIO_MIN 0x40  /* min expratio 1X */

#define ISP_DIGITAL_GAIN_SHIFT 8    /* ISP digital gain register shift */
#define ISP_DIGITAL_GAIN_MAX 0xFFF  /* max ISP digital gain 16X */
#define ISP_DIGITAL_GAIN_MIN 0x100  /* min ISP digital gain 1X */

#define DRC_STRENGTH_MAX 0xFF  /* max DRC strength 255 */
#define DRC_STRENGTH_MIN 0x0   /* min DRC strength 0 */

typedef struct hiISP_SYNC_CFG_BUF_S
{
    HI_U8                   u8BufWRFlag;    /*FW write node*/
    HI_U8                   u8BufRDFlag;    /*ISR read, then write to ISP reg or sensor*/
    ISP_SYNC_CFG_BUF_NODE_S  stSyncCfgBufNode[ISP_SYNC_BUF_NODE_NUM];
} ISP_SYNC_CFG_BUF_S;

typedef struct hiISP_SYNC_CFG_S
{
    HI_U8               u8WDRMode;
    HI_U8               u8PreWDRMode;
    HI_U8               u8VCNum;          /* if 3to1, u8VCNum = 0,1,2,0,1,......*/
    HI_U8               u8VCNumMax;       /* if 3to1, u8VCNumMax = 2 */
    HI_U8               u8VCCfgNum;
    HI_U8               u8Cfg2VldDlyMAX;

    HI_U32              u32DRCComp[CFG2VLD_DLY_LIMIT];  /* drc_collection_correction */
    HI_U32              u32ExpRatio[3][CFG2VLD_DLY_LIMIT];
    ISP_SYNC_CFG_BUF_NODE_S  *apstNode[CFG2VLD_DLY_LIMIT+1];

    ISP_SYNC_CFG_BUF_S   stSyncCfgBuf;
} ISP_SYNC_CFG_S;

typedef struct hiISP_DRV_DBG_INFO_S
{
    HI_U64 u64IspLastIntTime;           /* Time of last interrupt, for debug */
    HI_U64 u64IspLastRateTime;          /* Time of last interrupt rate, for debug */
    HI_U32 u32IspIntCnt;                /* Count of interrupt, for debug */
    HI_U32 u32IspIntTime;               /* Process time of interrupt, for debug */
    HI_U32 u32IspIntTimeMax;            /* Maximal process time of interrupt, for debug */
    HI_U32 u32IspIntGapTime;            /* Gap of two interrupts, for debug */
    HI_U32 u32IspIntGapTimeMax;         /* Maximal gap of two interrupts, for debug */
    HI_U32 u32IspRateIntCnt;            /* Count of interrupt rate, for debug */
    HI_U32 u32IspRate;                  /* Interrupt Rate, interrupt count per sencond, for debug */
    
    HI_U64 u64PtLastIntTime;            /* Time of last interrupt, for debug */
    HI_U64 u64PtLastRateTime;           /* Time of last interrupt rate, for debug */
    HI_U32 u32PtIntCnt;                 /* Count of interrupt, for debug */
    HI_U32 u32PtIntTime;                /* Process time of interrupt, for debug*/
    HI_U32 u32PtIntTimeMax;             /* Maximal process time of interrupt, for debug */
    HI_U32 u32PtIntGapTime;             /* Gap of two interrupts, for debug */
    HI_U32 u32PtIntGapTimeMax;          /* Maximal gap of two interrupts, for debug */
    HI_U32 u32PtRateIntCnt;             /* Count of interrupt rate, for debug */
    HI_U32 u32PtRate;                   /* Interrupt Rate, interrupt count per sencond, for debug */

    HI_U32 u32SensorCfgTime;            /* Time of sensor config, for debug */    
    HI_U32 u32SensorCfgTimeMax;         /* Maximal time of sensor config, for debug */    

    HI_U32 u32IspResetCnt;              /* Count of ISP reset when vi width or height changed */
} ISP_DRV_DBG_INFO_S;

typedef struct hiISP_INTERRUPT_SCH_S
{
    HI_U32 u32PortIntStatus;
    HI_U32 u32IspIntStatus;
    struct tasklet_struct tsklet;
    
} ISP_INTERRUPT_SCH_S;

typedef struct hiISP_DRV_CTX_S
{
    HI_U32  u32Status;
    HI_BOOL bEdge;
    HI_BOOL bVd;

    HI_BOOL bMemInit;
    
    ISP_WDR_CFG_S       stWDRCfg;
    ISP_SYNC_CFG_S      stSyncCfg;

    ISP_STAT_BUF_S      stStatisticsBuf;

    HI_U32              u32RegCfgInfoFlag;
    ISP_REG_CFG_S       stRegCfgInfo[2];        /* ping pong reg cfg info */

    ISP_BUS_CALLBACK_S  stBusCb;
    ISP_PIRIS_CALLBACK_S stPirisCb;

    ISP_DRV_DBG_INFO_S  stDrvDbgInfo;
#ifdef ENABLE_JPEGEDCF
    HI_U32              u32DCFInfoPhyAddr;
    ISP_DCF_INFO_S      *pDCFInfoVirAddr;
#endif
    ISP_PROC_MEM_S      stPorcMem;
    struct semaphore    stProcSem;

    ISP_INTERRUPT_SCH_S stIntSch;               /* isp interrupt schedule */
    
    wait_queue_head_t   stIspWait;
    wait_queue_head_t   stIspWaitVd;
    struct semaphore    stIspSem;
} ISP_DRV_CTX_S;

extern ISP_DRV_CTX_S   g_astIspDrvCtx[1];

#define ISP_DRV_GET_CTX(dev) (&g_astIspDrvCtx[dev])


/* * * * * * * * Buffer control begin * * * * * * * */
static HI_S32 __inline ISPSyncBufInit(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    memset(pstSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
    return 0;
}

static HI_S32 __inline ISPSyncBufIsFull(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return ((pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufRDFlag;
}

static HI_S32 __inline ISPSyncBufIsEmpty(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return pstSyncCfgBuf->u8BufWRFlag == pstSyncCfgBuf->u8BufRDFlag;
}

static HI_S32 __inline ISPSyncBufIsErr(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf))
    {
        return 0;
    }

    if (((pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufWRFlag)
    {
        return 0;
    }

    return 1;
}

static HI_S32 __inline ISPSyncBufWrite(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, ISP_SYNC_CFG_BUF_NODE_S *pstSyncCfgBufNode)
{
    if (ISPSyncBufIsFull(pstSyncCfgBuf))
    {
        return -1;
    }

    memcpy(&pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag], pstSyncCfgBufNode, sizeof(ISP_SYNC_CFG_BUF_NODE_S));
    pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static HI_S32 __inline ISPSyncBufRead(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, ISP_SYNC_CFG_BUF_NODE_S **ppstSyncCfgBufNode)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf))
    {
        return -1;
    }

    *ppstSyncCfgBufNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufRDFlag];
    pstSyncCfgBuf->u8BufRDFlag = (pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static HI_S32 __inline ISPSyncBufRead2(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, ISP_SYNC_CFG_BUF_NODE_S **ppstSyncCfgBufNode)
{
    HI_U8 u8Tmp = 0;

    if (ISPSyncBufIsEmpty(pstSyncCfgBuf))
    {
        return -1;
    }

    u8Tmp = (pstSyncCfgBuf->u8BufWRFlag + ISP_SYNC_BUF_NODE_NUM - 1) % ISP_SYNC_BUF_NODE_NUM;

    *ppstSyncCfgBufNode = &pstSyncCfgBuf->stSyncCfgBufNode[u8Tmp];
    pstSyncCfgBuf->u8BufRDFlag = (u8Tmp + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

/* * * * * * * * Buffer control end * * * * * * * */


//------------------------------------------------------------------------------
// isp sync task 
extern void SyncTaskInit(ISP_DEV dev);
HI_S32 IspSyncTaskProcess(ISP_DEV dev);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_H__ */

