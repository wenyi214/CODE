#include "Comment.h"
#include "ThreadCache.h"


void* ConcurrentAlloc(size_t size){
	if (size > MAX_BYTES){
		//ÏòpagecacheÉêÇë
	}
	else{
		//ÏòthreadcacheÉêÇë
		if (tls_threadcache == nullptr){
			tls_threadcache = new ThreadCache;
		}
		return tls_threadcache->Allocate(size);
	}
}

void ConcurrentFree(void *ptr, size_t size){
	assert(tls_threadcache);
	if (size > MAX_BYTES){
		//ÕÒpagecacheÊÍ·Å¿Õ¼ä
	}
	else{
		//ÕÒthreadcacheÊÍ·Å¿Õ¼ä
		tls_threadcache->Deallocate(ptr, size);
	}
	
}