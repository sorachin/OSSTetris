#include "Array.h"
#include <stdlib.h>
#include <memory.h>	
#include <stdio.h>
#include <time.h>

void Array_Create(Array* array, int capacity, size_t size){
	array->front = calloc(capacity, size);
	array->capacity = capacity;
	array->length = 0;
	array->current = -1;
}
////리스트 생성 함수
void Array_Destroy(Array* array){
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
}
//리스트 삭제 함수
int Array_Store(Array* array, int index, void* object, size_t size){
	memcpy(((char*)array->front) + (index * size), object, size);
	array->length++;
	array->current = index;
	return array->current;
}
//해당 리스트가 가진 메모리를 복사해서 데이터를 한칸 더 늘린 후 current인수에 저장
int Array_Insert(Array* array, int index, void* object, size_t size){
	void* temp;
	int i;
	int j = 0;
	temp = calloc(array->capacity + 1, size);
	for (i = 0; i < index; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	j = index + 1;
	for (i = index; i < array->capacity; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity++;
	memcpy(((char*)array->front) + (index * size), object, size);
	array->length++;
	array->current = index;
	return array->current;
}
//리스트 삽입함수
int Array_AppendFromFront(Array* array, void* object, size_t size){
	void* temp;
	int i;
	int j = 1;
	temp = calloc(array->capacity + 1, size);
	for (i = 0; i < array->capacity; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity++;
	memcpy(((char*)array->front) + (0 * size), object, size);
	array->length++;
	array->current = 0;
	return array->current;
}
//앞에서부터 데이터 삽입하는 함수
int Array_AppendFromRear(Array* array, void* object, size_t size){
	void* temp;
	int i;
	int j = 0;
	temp = calloc(array->capacity + 1, size);
	for (i = 0; i < array->capacity; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity++;
	memcpy(((char*)array->front) + ((array->capacity - 1) * size), object, size);
	array->length++;
	array->current = array->capacity - 1;
	return array->current;
}
//맨 뒤에 데이터를 삽입하는 함수
int Array_Delete(Array* array, int index, size_t size){
	void* temp;
	int i;
	int j = 0;
	temp = calloc(array->capacity - 1, size);
	for (i = 0; i < index; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	for (i = index + 1; i < array->capacity; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity--;
	array->length--;
	index = -1;
	array->current = index;
	return array->current;
}
//받아온 인덱스 부분까지 리스트 데이터를 삭제하는 함수
int Array_DeleteFromFront(Array* array, size_t size){
	void* temp;
	int i;
	int j = 0;
	temp = calloc(array->capacity - 1, size);
	for (i = 1; i < array->capacity; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity--;
	array->length--;
	array->current = -1;
	return array->current;
}
//리스트 맨 앞에있는 데이터를 삭제하는 함수
int Array_DeleteFromRear(Array* array, size_t size){
	void* temp;
	int i;
	int j = 0;
	temp = calloc(array->capacity - 1, size);
	for (i = 0; i < array->capacity - 1; i++){
		memcpy(((char*)temp) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
	}
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->front = temp;
	array->capacity--;
	array->length--;
	array->current = -1;
	return array->current;
}

void Array_Clear(Array* array){
	if (array->front){
		free(array->front);
		array->front = NULL;
	}
	array->capacity = 0;
	array->length = 0;
	array->current = -1;
}

int Array_Modify(Array* array, int index, void* object, size_t size) {
	memcpy(((char*)array->front) + (index * size), object, size);
	array->current = index;
	return array->current;
}
/*
//해당 리스트가 가진 메모리를 복사해서 current인수에 저장
int Array_LinearSearchByUnique(Array* array, void* key, size_t size, int(*Compare)(void*, void*)){
	int index = -1;
	int i = 0;
	while (i < array->length && Compare(((char*)array->front) + (i * size), key)){
		i++;
	}
	if (i < array->length){
		index = i;
	}
	array->current = index;
	return array->current;
}

void Array_LinearSearchByDuplicate(Array* array, void* key, int** indexes, int* count, size_t size, int(*Compare)(void*, void*)){
	int i;
	int j = 0;
	*count = 0;
	*indexes = (int*)calloc(array->length, sizeof(int));
	for (i = 0; i < array->length; i++){
		if (!Compare(((char*)array->front) + (i * size), key)){
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
	}
}

int Array_BinarySearchByUnique(Array* array, void* key, size_t size, int(*Compare)(void*, void*)){
	int index = -1;
	int left = 0;
	int right = array->length - 1;
	int middle = (left + right) / 2;
	while (left <= right && Compare(((char*)array->front) + (middle * size), key)){
		if (Compare(((char*)array->front) + (middle * size), key) < 0){
			left = middle + 1;
		}
		else{
			right = middle - 1;
		}
		middle = (left + right) / 2;
	}
	if (left <= right){
		index = middle;
	}
	array->current = index;
	return array->current;
}

void Array_BinarySearchByDuplicate(Array* array, void* key, int** indexes, int* count, size_t size, int(*Compare)(void*, void*)){
	int left = 0;
	int right = array->length - 1;
	int middle = (left + right) / 2;
	int j = 0;
	*count = 0;
	*indexes = (int*)calloc(array->length, sizeof(int));
	while (left <= right && Compare(((char*)array->front) + (middle * size), key)){
		if (Compare(((char*)array->front) + (middle * size), key) < 0){
			left = middle + 1;
		}
		else{
			right = middle - 1;
		}
		middle = (left + right) / 2;
	}
	if (left <= right){
		while (!Compare(((char*)array->front) + (middle * size), key)){
			middle--;
		}
		middle++;
		while (!Compare(((char*)array->front) + (middle * size), key)){
			(*indexes)[j] = middle;
			j++;
			(*count)++;
			middle++;
		}
	}
}
*/
void Array_SelectionSort(Array* array, size_t size, int(*Compare)(void*, void*)){
	void* tempObject;
	int i;
	int j;
	int minIndex;
	tempObject = calloc(1, size);
	for (i = 0; i < array->length - 1; i++){
		minIndex = i;
		memcpy(tempObject, ((char*)array->front) + (i * size), size);
		for (j = i + 1; j < array->length; j++){
			if (Compare(tempObject, ((char*)array->front) + (j * size)) > 0){
				memcpy(tempObject, ((char*)array->front) + (j * size), size);
				minIndex = j;
			}
		}
		memcpy(((char*)array->front) + (minIndex * size), ((char*)array->front) + (i * size), size);
		memcpy(((char*)array->front) + (i * size), tempObject, size);
	}
	if (tempObject){
		free(tempObject);
		tempObject = NULL;
	}
}
//리스트 선택정렬
void Array_InsertionSort(Array* array, size_t size, int(*Compare)(void*, void*)){
	void* tempObject;
	int i;
	int j;
	tempObject = calloc(1, size);
	for (i = 1; i < array->length; i++){
		memcpy(tempObject, ((char*)array->front) + (i * size), size);
		j = i;
		while (j > 0 && Compare(((char*)array->front) + ((j - 1) * size), tempObject) > 0){
			memcpy(((char*)array->front) + (j * size), ((char*)array->front) + ((j - 1) * size), size);
			j--;
		}
		memcpy(((char*)array->front) + (j * size), tempObject, size);
	}
	if (tempObject){
		free(tempObject);
		tempObject = NULL;
	}
}
//삽입정렬함수
void Array_Indirect_InsertionSort(Array* array, int** indexes, size_t size, int(*Compare)(void*, void*)){
	int i;
	int j;
	int temp;
	*indexes = (int*)calloc(array->length, sizeof(int));
	for (i = 0; i < array->length; i++){
		(*indexes)[i] = i;
	}
	for (i = 1; i < array->length; i++){
		temp = (*indexes)[i];
		j = i;
		while (j > 0 && Compare(((char*)array->front) + ((*indexes)[j - 1] * size), ((char*)array->front) + (temp * size)) > 0){
			(*indexes)[j] = (*indexes)[j - 1];
			j--;
		}
		(*indexes)[j] = temp;
	}
}
//간접정렬함수
void Array_GetAt(Array* array, int index, void* object, size_t size){
	memcpy(object, ((char*)array->front) + (index * size), size);
}
//리스트에 담긴 데이터를 가져오는함수
