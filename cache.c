#include <stdio.h>

// THE CACHE
struct LINE{
    char valid;
    int tag;
    char data[32];
};

struct SET{
    struct LINE line[1];
};

struct CACHE {
    struct SET set[8];
} cache;

// GLOBALDATA
// increment these in your function
unsigned hit_count  = 0;
unsigned miss_count = 0;
unsigned read_data_count = 0;

// SYSTEM BUS 
// call Read_Data_From_Ram to update this
char system_bus[32];


// READ DATA FROM RAM
// this function copies 32 character from the text file to
// the system_bus array aligned with the system bus
// (we will not test this with input larger than the text file)
void Read_Data_From_Ram(unsigned address) {
    address >>= 5; 
    address <<= 5; // get align the data
    read_data_count++;
    FILE *file;
    file = fopen("alice_in_wonderland.txt", "r");
    fseek(file, address, SEEK_SET);
    for (int i = 0; i<32; i++) 
        system_bus[i] = fgetc(file);
    return;
}

char Read_Data_From_Cache(unsigned address){
    
    //Extract the tag, set, and byte data from the address
    int tag = address / 256;
    int set = (address / 32) % 8;
    int byte = address % 32;
    
    //check to see if the data in the cache is valid and the tag matches
    // if so it should return the char matching the address and increment the hit count
    if (cache.set[set].line[0].valid && cache.set[set].line[0].tag == tag){
        hit_count++;
        return cache.set[set].line[0].data[byte];
    }

    // if not it should read the data from the system bus copy it into the cache and 
    // increment the miss count and return the char matching the address 
    miss_count++;
    Read_Data_From_Ram(address);

    cache.set[set].line[0].valid = 1;
    cache.set[set].line[0].tag = tag;
    for(int i = 0; i < 32; i++)
        cache.set[set].line[0].data[i] = system_bus[i];
    return cache.set[set].line[0].data[byte];   
}

//Print out the cache and the system bus for debug purpose
void print_cache(){
    printf("##############################################################\n");
    printf("Cache: \n");
    printf("%15s %8s %32s\n", "valid",  "tag", "data");
    for(int i = 0; i<8; i++)
        printf("set %d:%8d %8d    %s\n", i, cache.set[i].line[0].valid,  cache.set[i].line[0].tag, cache.set[i].line[0].data);
    printf("\n\n");
    printf("System bus: \n");
    for(int i=0; i<32; i++)
        printf("%c",system_bus[i]);
    printf("\n");
    printf("##############################################################\n");
    printf("\n");
}

int main() {
    
    // INITIALIZE CACHE
    for (int i = 0; i<8; i++) 
        cache.set[i].line[0].valid = 0;
    
    // READ SOME DATA
    char c;
    c = Read_Data_From_Cache(0); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(1); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(2); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(3); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(4); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    
    // WRITE A LOT MORE TESTS
    c = Read_Data_From_Cache(63); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(64); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    c = Read_Data_From_Cache(256); 
    printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );

#if 0
   for (int i= 0; i< 10; i++ ){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
   }

    for (int i=0; i<50; i+=10){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<500; i+=32){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<64; i+=16){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }
    for (int i=0; i<1000; i+=100){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<10000; i+=1000){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<100; i+=31){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<100; i+=29){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }

    for (int i=0; i<100; i+=15){
        char c = Read_Data_From_Cache(i); 
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count,read_data_count );
    }
#endif

    return 0;
}

