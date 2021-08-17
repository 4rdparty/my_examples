#include "gtest/gtest.h"
#include "libuv/include/uv.h"

//#####################################################################################

TEST(LibuvTest, CreateEventLoop) {
  // This program quits immediately because it has no events to process.
  // A libuv event loop has to be told to watch out for events using the various
  // API functions.
  uv_loop_t loop;
  int err = uv_loop_init(&loop);
  EXPECT_EQ(0, err);
  err = uv_run(&loop, UV_RUN_DEFAULT);
  EXPECT_EQ(0, err);
  err = uv_loop_close(&loop);
  EXPECT_EQ(0, err);
}

//#####################################################################################

TEST(LibuvTest, LoopWithIdling) {
  uv_idle_t idler;

  auto wait_for_a_while = [](uv_idle_t* handle) {
    static int64_t counter = 0;
    counter++;
    std::cout << counter << " ";
    if (counter >= 20) {
      EXPECT_EQ(20, counter);
      uv_idle_stop(handle);
    }
  };

  int err = uv_idle_init(uv_default_loop(), &idler);
  EXPECT_EQ(0, err);
  err = uv_idle_start(&idler, wait_for_a_while);
  EXPECT_EQ(0, err);

  err = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  EXPECT_EQ(0, err);

  err = uv_loop_alive(uv_default_loop());
  EXPECT_EQ(0, err);
  // Releases all internal loop resources. Call this function only when the loop
  // has finished executing and all open handles and requests have been closed,
  // or it will return UV_EBUSY. After this function returns, the user can free
  // the memory allocated for the loop.

  err = uv_loop_close(uv_default_loop());
  EXPECT_EQ(-4082, err);
  if (err)
    std::cout << "\n"
              << "############## description (" << err
              << "): " << uv_strerror(err) << "\n";  // Error handling
}

//#####################################################################################

TEST(LibuvTest, OpenFile) {
  const char* filename =
      "D:/iTechArt/My_Trunk/stout-eventuals-libuv/somedata.txt";
  uv_fs_t open_req;
  uv_loop_t* loop = uv_default_loop();

  // callback function
  auto open_cb = [](uv_fs_t* req) {
    // callback function
    auto close_cb = [](uv_fs_t* req) {
      int result = req->result;
      if (result < 0) {
        printf("Error at closing file: %s\n", uv_strerror(result));
      } else {
        printf("Successfully closed file.\n");
      }
      EXPECT_EQ(0, result);
    };

    uv_fs_t close_req;
    int result = req->result;
    uv_loop_t* loop = req->loop;

    if (result < 0) {
      printf("Error at opening file: %s\n", uv_strerror(result));
    } else {
      // result is file handler > 0
      printf("Successfully opened file.\n");
    }
    result = uv_fs_close(loop, &close_req, req->result, close_cb);
    EXPECT_EQ(0, result);
    uv_fs_req_cleanup(req);
  };

  int result =
      uv_fs_open(loop, &open_req, filename, O_RDONLY, S_IREAD, open_cb);

  if (result < 0) {
    printf("Error at opening file: %s\n", uv_strerror(result));
  }
  EXPECT_EQ(0, result);

  printf("Event loop is started.\n");
  result = uv_run(loop, UV_RUN_DEFAULT);
  EXPECT_EQ(0, result);
}

//#####################################################################################

uv_loop_t* loop;

uv_fs_t open_req;
uv_fs_t close_req;
uv_fs_t exit_req;
uv_fs_t write_req;

void open_cb(uv_fs_t*);
void write_cb(uv_fs_t*);
void exit_cb(uv_fs_t*);

uv_buf_t buffer;

const char* filename =
    "D:/iTechArt/My_Trunk/stout-eventuals-libuv/somedata.txt";
const size_t len = 1024;
unsigned long offset = 0;

void write_cb(uv_fs_t* req) {
  int result = req->result;
  if (result < 0) {
    printf("Error at writing file: %s\n", uv_strerror(result));
  }
  printf("File is closed.\n");
  int r = uv_fs_close(loop, &close_req, req->file.fd, exit_cb);
  EXPECT_EQ(0, r);
}

void open_cb(uv_fs_t* req) {
  int result = req->result;

  if (result < 0) {
    printf("Error at opening file: %s\n", uv_strerror((int)req->result));
  }
  printf("Successfully opened file.\n");
  uv_fs_req_cleanup(req);
  printf("Write to file.\n");
  int r = uv_fs_write(loop, &write_req, result, &buffer, 1, 0, write_cb);
  EXPECT_EQ(0, r);
}

void exit_cb(uv_fs_t* req) {
  printf("exit.\n");
  free(buffer.base);
  uv_fs_req_cleanup(&write_req);
}

TEST(LibuvTest, SaveFile) {
  char* base = (char*)malloc(len);

  loop = uv_default_loop();
  buffer = uv_buf_init(base, len);
  //Заносим данные, которые надо записать, в буфер
  memcpy(base, "Some data to save", 17);
  //Указываем, сколько байт записать
  buffer.len = 17;

  printf("File is opened.\n");
  int r =
      uv_fs_open(loop, &open_req, filename, O_RDWR | O_CREAT, 0666, open_cb);
  if (r < 0) {
    printf("Error at opening file: %s\n", uv_strerror(r));
  }
  EXPECT_EQ(0, r);
  printf("Event loop is started.\n");
  uv_run(loop, UV_RUN_DEFAULT);
}