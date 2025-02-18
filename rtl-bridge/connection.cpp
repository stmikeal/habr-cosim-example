#include "Vtestbench__Dpi.h"
#include <endian.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include <cstdint>
#include <thread>
#include <vector>

static int fd;
int data;

static struct {
  int value;
  bool enable;
  long addr;
  volatile bool ping;
} transaction;

void sk_close() {
  shutdown(fd, SHUT_RDWR);
  close(fd);
}

static int readSocket(uint8_t* data, size_t size) {
  int ret = read(fd, data, size);
  if (ret <= 0)
    exit(1);
  return be32toh(((uint32_t*)data)[1]);
}

static int readPacket(uint8_t* packet) {
  int ret = readSocket(packet, 20);
  readSocket(packet + 20, ret);
  uint32_t* p = (uint32_t*)packet;
  return ret;
}

void writeSocket(uint8_t* data, size_t size) { write(fd, data, size); }

void writePacket(uint32_t* data) {
  writeSocket((uint8_t*)data, 20 + be32toh(data[1]));
}

int accept_transaction() {
  if (transaction.enable) {
    transaction.enable = false;
      sv_write(transaction.addr, transaction.value);
  }
  return 0;
}

void onWriteOperation(long addr, int value) {
  transaction.value = value;
  transaction.addr = addr;
  transaction.ping = false;
  transaction.enable = true;
  while (!transaction.ping) {}
}

void handleRequest(uint32_t* data) {
  switch (be32toh(data[0])) {
    case 4:  // write
      onWriteOperation((be32toh(data[9]) << 32) + be32toh(data[10]),
                       ((uint32_t*)(((uint8_t*)data) + 2))[14]);
      data[1] = be32toh(be32toh(data[1]) - 4);
      data[3] = be32toh(2);
      writePacket(data);
      break;
    case 6:  // sync
      data[3] = be32toh(2);
      writePacket(data);
      break;
    default:
      return;
  }
}

static void readSerializer() {
  while (1) {
    std::vector<uint32_t> packet;
    packet.resize(30, 0);
    if (readPacket((uint8_t*)packet.data())) {
      handleRequest(packet.data());
    }
  }
}

void sk_connect() {
  int fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr_;
  addr_.sun_family = AF_UNIX;
  strcpy(addr_.sun_path, "/tmp/cosim.sock");
  connect(fd_, (sockaddr*)&addr_, sizeof(addr_));
  fd = fd_;
  std::thread(&readSerializer).detach();
}

int c_write_resp() {
  transaction.ping = 1;
  return 0;
}
