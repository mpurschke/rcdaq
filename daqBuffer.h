#ifndef __DAQBUFFER_H
#define __DAQBUFFER_H


#include <lzo/lzoutil.h>

#include <EvtStructures.h>
#include <daq_device.h>
#include <EventTypes.h>
#include <daqEvent.h>
#include <BufferConstants.h>
#include "md5.h"

#include <arpa/inet.h>


#define CTRL_BEGINRUN        1
#define CTRL_ENDRUN          2
#define CTRL_DATA            3
#define CTRL_CLOSE           4
#define CTRL_SENDFILENAME    5
#define CTRL_ROLLOVER        6

#define CTRL_REMOTESUCCESS 100
#define CTRL_REMOTEFAIL    101



class daqBuffer {

public:

  //** Constructors

  daqBuffer(const int irun = 1, const int length = 8*1024*1024+2*8192
	    , const int iseq = 1,   md5_state_t *md5state = 0);

  virtual ~daqBuffer();

  //  this creates a new event on the next address
  int nextEvent(const int etype, const int evtseq, const int maxsize);

  int prepare_next( const int iseq, const int irun =-1);

  // subevent adding
  unsigned int addSubevent( daq_device *);

  //  add end-of-buffer
  unsigned int addEoB();

  int setCompression(const int flag);
  int getCompression() const {return wants_compression;} ;
  
  // now the write routine
  unsigned int writeout ( int fd);

  // now the "send buffer to a server" routine
  unsigned int sendout ( int fd );
  
  // now the "send monitor data" routine
  unsigned int sendData ( int fd, const int max_length);

  // now the re-sizing of buffer
  int setMaxSize( const int size);

  // and the query
  int getMaxSize() const ;

  // and the query
  int getBufSeq () const { return bptr->Bufseq; } ;
  unsigned int getLength () const { return bptr->Length; } ;

  int setEventFormat(const int f);
  int getEventFormat() const {return format;};

  // MD5 checksum business
  void setMD5State( md5_state_t *md5state) {_md5state = md5state;};
  md5_state_t * getMD5State() const {return _md5state;};
  void setMD5Enabled(const int x) { if(x) md5_enabled =1; else md5_enabled =0; }
  int  getMD5Enabled() const { return md5_enabled; };

protected:

  // now the compress routine
  int compress ();
  
  typedef struct 
  { 
    unsigned int Length;
    int ID;
    int Bufseq;
    int Runnr;
    int data[1];
  } *buffer_ptr;

  int _broken;
  buffer_ptr bptr;
  int *data_ptr;
  int current_index;
  int max_length;
  int max_size;
  int left;
  daqEvent *current_event;
  int current_etype;
  int has_end;
  
  int format;
  int wants_compression;
  
  int currentBufferID;

  int md5_enabled;
  md5_state_t *_md5state;

  static int lzo_initialized;
  lzo_byte *wrkmem;
  lzo_uint  outputarraylength;
  unsigned int *outputarray;

};

#endif

