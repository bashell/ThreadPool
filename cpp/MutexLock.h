#ifndef _MUTEX_LOCK_H_
#define _MUTEX_LOCK_H_

#include <pthread.h>
#include <assert.h>

class MutexLock {
 public:
  MutexLock();
  ~MutexLock();

  void lock();
  void unlock();

  bool isLocked() const;
  pthread_mutex_t *getMutexPtr();

 private:
  MutexLock(const MutexLock&) = delete;
  MutexLock &operator=(const MutexLock&) = delete;

 private:
  pthread_mutex_t mutex_;
  bool isLocked_;
};

inline MutexLock::MutexLock() : isLocked_(false) {
  pthread_mutex_init(&mutex_, NULL);
}

inline MutexLock::~MutexLock() {
  assert(isLocked_ == false);  // 解锁后再销毁锁
  pthread_mutex_destroy(&mutex_);
}

inline void MutexLock::lock() {
  pthread_mutex_lock(&mutex_);
  isLocked_ = true;
}

inline void MutexLock::unlock() {
  isLocked_ = false;  // 解锁前设置锁的状态
  pthread_mutex_unlock(&mutex_);
}

inline bool MutexLock::isLocked() const {
  return isLocked_;
}

inline pthread_mutex_t *MutexLock::getMutexPtr() {
  return &mutex_;
}


#endif  /* _MUTEX_LOCK_H_ */
