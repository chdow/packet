// needed to make a node add-on
#include <node.h>
#include <node_buffer.h>
#include <v8.h>

extern "C" {
#include <stdint.h>
#include <string.h>
}

using namespace v8;
using namespace node;

// define data -- typically would be in a separate file.

// packet type
#define pcktCommand  0xC 
#define pcktResponse 0xA
#define pcktError    0xE
typedef uint8_t packetType_t;

// commands
#define cmdStatus  0
#define cmdUnlock  1
#define cmdLock    2
#define cmdGetTime 3
#define cmdSetTime 4
typedef unsigned char command_t;

typedef struct {
  packetType_t type;  
  command_t    command;
  uint16_t     pad_reserved_0;
  union {
    struct {
      uint32_t arg0;
      uint32_t arg1;
    } args;
    struct {
      uint32_t epochTimeInS;
      uint32_t reserved;
    } time;
  };
} __attribute__((packed)) cmdPacket_t;


void makeCommandPacket(packetType_t type, command_t command, cmdPacket_t *packet) {
  memset((void *) packet, 0, sizeof(cmdPacket_t));
  packet->type = type;
  packet->command = command;
}

// C++ Shim 

Handle<Value> status(const Arguments& args) {
  HandleScope scope;
  cmdPacket_t packet;

  makeCommandPacket(pcktCommand, cmdStatus, &packet);

  Local<Buffer>  pcktBuff = node::Buffer::New((char *) &packet, sizeof(packet));

  return scope.Close(pcktBuff->handle_);
  
}

Handle<Value> unlock(const Arguments& args) {
  HandleScope scope;
  cmdPacket_t packet;

  makeCommandPacket(pcktCommand, cmdUnlock, &packet);

  Local<Buffer> pcktBuff = node::Buffer::New((char *) &packet, sizeof(packet));

  return scope.Close(pcktBuff->handle_);

}

Handle<Value> lock(const Arguments& args) {
  HandleScope scope;
  cmdPacket_t packet;

  makeCommandPacket(pcktCommand, cmdLock, &packet);

  Local<Buffer> pcktBuff = node::Buffer::New((char *) &packet, sizeof(packet));

  return scope.Close(pcktBuff->handle_);
}

Handle<Value> getTime(const Arguments& args) {
  HandleScope scope;
  cmdPacket_t packet;

  makeCommandPacket(pcktCommand, cmdGetTime, &packet);

  Local<Buffer> pcktBuff = node::Buffer::New((char *) &packet, sizeof(packet));

  return scope.Close(pcktBuff->handle_);
}

Handle<Value> setTime(const Arguments& args) {
  HandleScope scope;
  cmdPacket_t packet;

  if ((args.Length() < 1) || !(args[0]->IsNumber())) {
    ThrowException(Exception::TypeError(String::New("setTime takes one argument and it must be a number")));
    return scope.Close(Undefined());
  }

  uint32_t epochTimeInS = (uint32_t) args[0]->NumberValue();

  makeCommandPacket(pcktCommand, cmdSetTime, &packet);
  packet.time.epochTimeInS = epochTimeInS;

  Local<Buffer> pcktBuff = node::Buffer::New((char *) &packet, sizeof(packet));

  return scope.Close(pcktBuff->handle_);
}

void init(Handle<Object> target) {
#define ADD_FUNCTION_TO_MODULE(name) \
  target->Set(String::NewSymbol((#name)), \
  FunctionTemplate::New((name))->GetFunction());

  ADD_FUNCTION_TO_MODULE(status);
  ADD_FUNCTION_TO_MODULE(unlock);
  ADD_FUNCTION_TO_MODULE(lock);
  ADD_FUNCTION_TO_MODULE(getTime);
  ADD_FUNCTION_TO_MODULE(setTime);
}

NODE_MODULE(packet, init)