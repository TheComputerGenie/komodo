
/******************************************************************************
 * Copyright © 2014-2019 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

// todo:

// headers "sync" make sure it connects to prior blocks to notarization. use getinfo hdrht to get missing hdrs

// interest calculations are currently just using what is returned, it should calculate it from scratch

// CC signing
// make sure to sanity check all vector lengths on receipt
// make sure no files are updated (this is to allow nSPV=1 and later nSPV=0 without affecting database)

#ifndef KOMODO_NSPV_H
#define KOMODO_NSPV_H

#define NSPV_POLLITERS 10
#define NSPV_POLLMICROS 100777
#define NSPV_MAXVINS 64
#define NSPV_AUTOLOGOUT 777
#define NSPV_BRANCHID 0x76b809bb

// nSPV defines and struct definitions with serialization and purge functions

#define NSPV_INFO 0x00
#define NSPV_INFORESP 0x01
#define NSPV_UTXOS 0x02
#define NSPV_UTXOSRESP 0x03
#define NSPV_NTZS 0x04
#define NSPV_NTZSRESP 0x05
#define NSPV_NTZSPROOF 0x06
#define NSPV_NTZSPROOFRESP 0x07
#define NSPV_TXPROOF 0x08
#define NSPV_TXPROOFRESP 0x09
#define NSPV_SPENTINFO 0x0a
#define NSPV_SPENTINFORESP 0x0b
#define NSPV_BROADCAST 0x0c
#define NSPV_BROADCASTRESP 0x0d

int32_t NSPV_gettransaction(int32_t skipvalidation,int32_t vout,uint256 txid,int32_t height,CTransaction &tx);
extern uint256 SIG_TXHASH;
uint32_t NSPV_blocktime(int32_t hdrheight);

int32_t iguana_rwbuf(int32_t rwflag,uint8_t *serialized,uint16_t len,uint8_t *buf)
{
    if ( rwflag != 0 )
        memcpy(serialized,buf,len);
    else memcpy(buf,serialized,len);
    return(len);
}

struct NSPV_equihdr
{
    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint256 hashFinalSaplingRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint256 nNonce;
    uint8_t nSolution[1344];
};

int32_t NSPV_rwequihdr(int32_t rwflag,uint8_t *serialized,struct NSPV_equihdr *ptr)
{
    int32_t len = 0;
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nVersion),&ptr->nVersion);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->hashPrevBlock),(uint8_t *)&ptr->hashPrevBlock);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->hashMerkleRoot),(uint8_t *)&ptr->hashMerkleRoot);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->hashFinalSaplingRoot),(uint8_t *)&ptr->hashFinalSaplingRoot);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nTime),&ptr->nTime);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nBits),&ptr->nBits);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->nNonce),(uint8_t *)&ptr->nNonce);
    len += iguana_rwbuf(rwflag,&serialized[len],sizeof(ptr->nSolution),ptr->nSolution);
    return(len);
}

int32_t iguana_rwequihdrvec(int32_t rwflag,uint8_t *serialized,uint16_t *vecsizep,struct NSPV_equihdr **ptrp)
{
    int32_t i,vsize,len = 0;
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(*vecsizep),vecsizep);
    if ( (vsize= *vecsizep) != 0 )
    {
        //fprintf(stderr,"vsize.%d ptrp.%p alloc %ld\n",vsize,*ptrp,sizeof(struct NSPV_equihdr)*vsize);
        if ( *ptrp == 0 )
            *ptrp = (struct NSPV_equihdr *)calloc(sizeof(struct NSPV_equihdr),vsize); // relies on uint16_t being "small" to prevent mem exhaustion
        for (i=0; i<vsize; i++)
            len += NSPV_rwequihdr(rwflag,&serialized[len],&(*ptrp)[i]);
    }
    return(len);
}

int32_t iguana_rwuint8vec(int32_t rwflag,uint8_t *serialized,uint16_t *vecsizep,uint8_t **ptrp)
{
    int32_t vsize,len = 0;
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(*vecsizep),vecsizep);
    if ( (vsize= *vecsizep) != 0 )
    {
        if ( *ptrp == 0 )
            *ptrp = (uint8_t *)calloc(1,vsize); // relies on uint16_t being "small" to prevent mem exhaustion
        len += iguana_rwbuf(rwflag,&serialized[len],vsize,*ptrp);
    }
    return(len);
}

struct NSPV_utxoresp
{
    uint256 txid;
    int64_t satoshis,extradata;
    int32_t vout,height;
};

int32_t NSPV_rwutxoresp(int32_t rwflag,uint8_t *serialized,struct NSPV_utxoresp *ptr)
{
    int32_t len = 0;
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->txid),(uint8_t *)&ptr->txid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->satoshis),&ptr->satoshis);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->extradata),&ptr->extradata);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->vout),&ptr->vout);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->height),&ptr->height);
    return(len);
}

struct NSPV_utxosresp
{
    struct NSPV_utxoresp *utxos;
    char coinaddr[64];
    int64_t total,interest;
    int32_t nodeheight;
    uint16_t numutxos; uint8_t CCflag,pad8;
};

int32_t NSPV_rwutxosresp(int32_t rwflag,uint8_t *serialized,struct NSPV_utxosresp *ptr) // check mempool
{
    int32_t i,len = 0;
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->numutxos),&ptr->numutxos);
    if ( ptr->numutxos != 0 )
    {
        if ( ptr->utxos == 0 )
            ptr->utxos = (struct NSPV_utxoresp *)calloc(sizeof(*ptr->utxos),ptr->numutxos); // relies on uint16_t being "small" to prevent mem exhaustion
        for (i=0; i<ptr->numutxos; i++)
            len += NSPV_rwutxoresp(rwflag,&serialized[len],&ptr->utxos[i]);
    }
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->total),&ptr->total);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->interest),&ptr->interest);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nodeheight),&ptr->nodeheight);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->CCflag),&ptr->CCflag);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->pad8),&ptr->pad8);
    if ( rwflag != 0 )
    {
        memcpy(&serialized[len],ptr->coinaddr,sizeof(ptr->coinaddr));
        len += sizeof(ptr->coinaddr);
    }
    else
    {
        memcpy(ptr->coinaddr,&serialized[len],sizeof(ptr->coinaddr));
        len += sizeof(ptr->coinaddr);
    }
    return(len);
}

void NSPV_utxosresp_purge(struct NSPV_utxosresp *ptr)
{
    if ( ptr != 0 )
    {
        if ( ptr->utxos != 0 )
            free(ptr->utxos);
        memset(ptr,0,sizeof(*ptr));
    }
}

struct NSPV_ntz
{
    uint256 blockhash,txid,othertxid;
    int32_t height,txidheight;
};

int32_t NSPV_rwntz(int32_t rwflag,uint8_t *serialized,struct NSPV_ntz *ptr)
{
    int32_t len = 0;
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->blockhash),(uint8_t *)&ptr->blockhash);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->txid),(uint8_t *)&ptr->txid);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->othertxid),(uint8_t *)&ptr->othertxid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->height),&ptr->height);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->txidheight),&ptr->txidheight);
    return(len);
}

struct NSPV_ntzsresp
{
    struct NSPV_ntz prevntz,nextntz;
};

int32_t NSPV_rwntzsresp(int32_t rwflag,uint8_t *serialized,struct NSPV_ntzsresp *ptr)
{
    int32_t len = 0;
    len += NSPV_rwntz(rwflag,&serialized[len],&ptr->prevntz);
    len += NSPV_rwntz(rwflag,&serialized[len],&ptr->nextntz);
    return(len);
}

void NSPV_ntzsresp_purge(struct NSPV_ntzsresp *ptr)
{
    if ( ptr != 0 )
        memset(ptr,0,sizeof(*ptr));
}

struct NSPV_inforesp
{
    struct NSPV_ntz notarization;
    uint256 blockhash;
    int32_t height,hdrheight;
    struct NSPV_equihdr H;
};

int32_t NSPV_rwinforesp(int32_t rwflag,uint8_t *serialized,struct NSPV_inforesp *ptr)
{
    int32_t len = 0;
    len += NSPV_rwntz(rwflag,&serialized[len],&ptr->notarization);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->blockhash),(uint8_t *)&ptr->blockhash);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->height),&ptr->height);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->hdrheight),&ptr->hdrheight);
    len += NSPV_rwequihdr(rwflag,&serialized[len],&ptr->H);
    //fprintf(stderr,"hdr rwlen.%d\n",len);
    return(len);
}

void NSPV_inforesp_purge(struct NSPV_inforesp *ptr)
{
    if ( ptr != 0 )
        memset(ptr,0,sizeof(*ptr));
}

struct NSPV_txproof
{
    uint256 txid;
    int64_t unspentvalue;
    int32_t height,vout,pad;
    uint16_t txlen,txprooflen;
    uint8_t *tx,*txproof;
};

int32_t NSPV_rwtxproof(int32_t rwflag,uint8_t *serialized,struct NSPV_txproof *ptr)
{
    int32_t len = 0;
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->txid),(uint8_t *)&ptr->txid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->unspentvalue),&ptr->unspentvalue);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->height),&ptr->height);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->vout),&ptr->vout);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->pad),&ptr->pad);
    len += iguana_rwuint8vec(rwflag,&serialized[len],&ptr->txlen,&ptr->tx);
    len += iguana_rwuint8vec(rwflag,&serialized[len],&ptr->txprooflen,&ptr->txproof);
    return(len);
}

void NSPV_txproof_purge(struct NSPV_txproof *ptr)
{
    if ( ptr != 0 )
    {
        if ( ptr->tx != 0 )
            free(ptr->tx);
        if ( ptr->txproof != 0 )
            free(ptr->txproof);
        memset(ptr,0,sizeof(*ptr));
    }
}

struct NSPV_ntzproofshared
{
    struct NSPV_equihdr *hdrs;
    int32_t prevht,nextht,pad32;
    uint16_t numhdrs,pad16;
};

int32_t NSPV_rwntzproofshared(int32_t rwflag,uint8_t *serialized,struct NSPV_ntzproofshared *ptr)
{
    int32_t len = 0;
    len += iguana_rwequihdrvec(rwflag,&serialized[len],&ptr->numhdrs,&ptr->hdrs);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->prevht),&ptr->prevht);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nextht),&ptr->nextht);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->pad32),&ptr->pad32);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->pad16),&ptr->pad16);
    //fprintf(stderr,"rwcommon prev.%d next.%d\n",ptr->prevht,ptr->nextht);
    return(len);
}

struct NSPV_ntzsproofresp
{
    struct NSPV_ntzproofshared common;
    uint256 prevtxid,nexttxid;
    int32_t pad32,prevtxidht,nexttxidht;
    uint16_t prevtxlen,nexttxlen;
    uint8_t *prevntz,*nextntz;
};

int32_t NSPV_rwntzsproofresp(int32_t rwflag,uint8_t *serialized,struct NSPV_ntzsproofresp *ptr)
{
    int32_t len = 0;
    len += NSPV_rwntzproofshared(rwflag,&serialized[len],&ptr->common);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->prevtxid),(uint8_t *)&ptr->prevtxid);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->nexttxid),(uint8_t *)&ptr->nexttxid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->pad32),&ptr->pad32);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->prevtxidht),&ptr->prevtxidht);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->nexttxidht),&ptr->nexttxidht);
    len += iguana_rwuint8vec(rwflag,&serialized[len],&ptr->prevtxlen,&ptr->prevntz);
    len += iguana_rwuint8vec(rwflag,&serialized[len],&ptr->nexttxlen,&ptr->nextntz);
    return(len);
}

void NSPV_ntzsproofresp_purge(struct NSPV_ntzsproofresp *ptr)
{
    if ( ptr != 0 )
    {
        if ( ptr->common.hdrs != 0 )
            free(ptr->common.hdrs);
        if ( ptr->prevntz != 0 )
            free(ptr->prevntz);
        if ( ptr->nextntz != 0 )
            free(ptr->nextntz);
        memset(ptr,0,sizeof(*ptr));
    }
}

struct NSPV_MMRproof
{
    struct NSPV_ntzproofshared common;
    // tbd
};

struct NSPV_spentinfo
{
    struct NSPV_txproof spent;
    uint256 txid;
    int32_t vout,spentvini;
};

int32_t NSPV_rwspentinfo(int32_t rwflag,uint8_t *serialized,struct NSPV_spentinfo *ptr) // check mempool
{
    int32_t len = 0;
    len += NSPV_rwtxproof(rwflag,&serialized[len],&ptr->spent);
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->txid),(uint8_t *)&ptr->txid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->vout),&ptr->vout);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->spentvini),&ptr->spentvini);
    return(len);
}

void NSPV_spentinfo_purge(struct NSPV_spentinfo *ptr)
{
    if ( ptr != 0 )
    {
        NSPV_txproof_purge(&ptr->spent);
        memset(ptr,0,sizeof(*ptr));
    }
}

struct NSPV_broadcastresp
{
    uint256 txid;
    int32_t retcode;
};

int32_t NSPV_rwbroadcastresp(int32_t rwflag,uint8_t *serialized,struct NSPV_broadcastresp *ptr)
{
    int32_t len = 0;
    len += iguana_rwbignum(rwflag,&serialized[len],sizeof(ptr->txid),(uint8_t *)&ptr->txid);
    len += iguana_rwnum(rwflag,&serialized[len],sizeof(ptr->retcode),&ptr->retcode);
    return(len);
}

void NSPV_broadcast_purge(struct NSPV_broadcastresp *ptr)
{
    if ( ptr != 0 )
        memset(ptr,0,sizeof(*ptr));
}

// useful utility functions

uint256 NSPV_doublesha256(uint8_t *data,int32_t datalen)
{
    bits256 _hash; uint256 hash; int32_t i;
    _hash = bits256_doublesha256(0,data,datalen);
    for (i=0; i<32; i++)
        ((uint8_t *)&hash)[i] = _hash.bytes[31 - i];
    return(hash);
}

uint256 NSPV_hdrhash(struct NSPV_equihdr *hdr)
{
    CBlockHeader block;
    block.nVersion = hdr->nVersion;
    block.hashPrevBlock = hdr->hashPrevBlock;
    block.hashMerkleRoot = hdr->hashMerkleRoot;
    block.hashFinalSaplingRoot = hdr->hashFinalSaplingRoot;
    block.nTime = hdr->nTime;
    block.nBits = hdr->nBits;
    block.nNonce = hdr->nNonce;
    block.nSolution.resize(sizeof(hdr->nSolution));
    memcpy(&block.nSolution[0],hdr->nSolution,sizeof(hdr->nSolution));
    return(block.GetHash());
}

int32_t NSPV_txextract(CTransaction &tx,uint8_t *data,int32_t datalen)
{
    std::vector<uint8_t> rawdata;
    rawdata.resize(datalen);
    memcpy(&rawdata[0],data,datalen);
    if ( DecodeHexTx(tx,HexStr(rawdata)) != 0 )
        return(0);
    else return(-1);
}

bool NSPV_SignTx(CMutableTransaction &mtx,int32_t vini,int64_t utxovalue,const CScript scriptPubKey);

int32_t NSPV_fastnotariescount(CTransaction tx,uint8_t elected[64][33])
{
    CPubKey pubkeys[64]; uint8_t sig[512]; CScript scriptPubKeys[64]; CMutableTransaction mtx(tx); int32_t vini,j,siglen,retval; uint64_t mask = 0; char *str; std::vector<std::vector<unsigned char>> vData;
    for (j=0; j<64; j++)
    {
        pubkeys[j] = buf2pk(elected[j]);
        scriptPubKeys[j] = (CScript() << ParseHex(HexStr(pubkeys[j])) << OP_CHECKSIG);
        //fprintf(stderr,"%d %s\n",j,HexStr(pubkeys[j]).c_str());
    }
    fprintf(stderr,"txid %s\n",tx.GetHash().GetHex().c_str());
    //for (vini=0; vini<tx.vin.size(); vini++)
    //    mtx.vin[vini].scriptSig.resize(0);
    for (vini=0; vini<tx.vin.size(); vini++)
    {
        CScript::const_iterator pc = tx.vin[vini].scriptSig.begin();
        if ( tx.vin[vini].scriptSig.GetPushedData(pc,vData) != 0 )
        {
            vData[0].pop_back();
            for (j=0; j<64; j++)
            {
                if ( ((1LL << j) & mask) != 0 )
                    continue;
                char coinaddr[64]; Getscriptaddress(coinaddr,scriptPubKeys[j]);
                NSPV_SignTx(mtx,vini,10000,scriptPubKeys[j]); // sets SIG_TXHASH
                if ( (retval= pubkeys[j].Verify(SIG_TXHASH,vData[0])) != 0 )
                {
                    fprintf(stderr,"(vini.%d %s.%d) ",vini,coinaddr,retval);
                    mask |= (1LL << j);
                    break;
                }
            }
            fprintf(stderr," verified %llx\n",(long long)mask);
        }
    }
    return(bitweight(mask));
}

/*
 NSPV_notariescount is the slowest process during full validation as it requires looking up 13 transactions.
 one way that would be 10000x faster would be to bruteforce validate the signatures in each vin, against all 64 pubkeys! for a valid tx, that is on average 13*32 secp256k1/sapling verify operations, which is much faster than even a single network request.
 Unfortunately, due to the complexity of calculating the hash to sign for a tx, this bruteforcing would require determining what type of signature method and having sapling vs legacy methods of calculating the txhash.
 It could be that the fullnode side could calculate this and send it back to the superlite side as any hash that would validate 13 different ways has to be the valid txhash.
 However, since the vouts being spent by the notaries are highly constrained p2pk vouts, the txhash can be deduced if a specific notary pubkey is indeed the signer
 */
int32_t NSPV_notariescount(CTransaction tx,uint8_t elected[64][33])
{
    uint8_t *script; CTransaction vintx; int32_t i,j,utxovout,scriptlen,numsigs = 0;
    for (i=0; i<tx.vin.size(); i++)
    {
        utxovout = tx.vin[i].prevout.n;
        if ( NSPV_gettransaction(1,utxovout,tx.vin[i].prevout.hash,0,vintx) != 0 )
        {
            fprintf(stderr,"error getting %s/v%d\n",tx.vin[i].prevout.hash.GetHex().c_str(),utxovout);
            return(numsigs);
        }
        if ( utxovout < vintx.vout.size() )
        {
            script = (uint8_t *)&vintx.vout[utxovout].scriptPubKey[0];
            if ( (scriptlen= vintx.vout[utxovout].scriptPubKey.size()) == 35 )
            {
                for (j=0; j<64; j++)
                    if ( memcmp(&script[1],elected[j],33) == 0 )
                    {
                        numsigs++;
                        break;
                    }
            } else fprintf(stderr,"invalid scriptlen.%d\n",scriptlen);
        } else fprintf(stderr,"invalid utxovout.%d vs %d\n",utxovout,(int32_t)vintx.vout.size());
    }
    return(numsigs);
}

uint256 NSPV_opretextract(int32_t *heightp,uint256 *blockhashp,char *symbol,std::vector<uint8_t> opret,uint256 txid)
{
    uint256 desttxid; int32_t i;
    iguana_rwnum(0,&opret[32],sizeof(*heightp),heightp);
    for (i=0; i<32; i++)
        ((uint8_t *)blockhashp)[i] = opret[i];
    for (i=0; i<32; i++)
        ((uint8_t *)&desttxid)[i] = opret[4 + 32 + i];
    if ( 0 && *heightp != 2690 )
        fprintf(stderr," ntzht.%d %s <- txid.%s size.%d\n",*heightp,(*blockhashp).GetHex().c_str(),(txid).GetHex().c_str(),(int32_t)opret.size());
    return(desttxid);
}

int32_t NSPV_notarizationextract(int32_t verifyntz,int32_t *ntzheightp,uint256 *blockhashp,uint256 *desttxidp,CTransaction tx)
{
    int32_t numsigs=0; uint8_t elected[64][33]; char *symbol; std::vector<uint8_t> opret;
    if ( tx.vout.size() >= 2 )
    {
        symbol = (ASSETCHAINS_SYMBOL[0] == 0) ? (char *)"KMD" : ASSETCHAINS_SYMBOL;
        GetOpReturnData(tx.vout[1].scriptPubKey,opret);
        if ( opret.size() >= 32*2+4 )
        {
            sleep(1);
            *desttxidp = NSPV_opretextract(ntzheightp,blockhashp,symbol,opret,tx.GetHash());
            komodo_notaries(elected,*ntzheightp,NSPV_blocktime(*ntzheightp));
            if ( verifyntz != 0 && (numsigs= NSPV_fastnotariescount(tx,elected)) < 12 )
            {
                fprintf(stderr,"numsigs.%d error\n",numsigs);
                return(-3);
            }
            return(0);
        }
        else
        {
            fprintf(stderr,"opretsize.%d error\n",(int32_t)opret.size());
            return(-2);
        }
    } else return(-1);
}
#endif // KOMODO_NSPV_H