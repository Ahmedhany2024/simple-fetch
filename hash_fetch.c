
#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>



int cpu_info(void){

	FILE *file;

	char buffer[1024];

	char name[256];
	
	char model[256];

	file = fopen("/proc/cpuinfo", "r");

	if (file){

	while(fgets(buffer , sizeof(buffer), file)){
		if ((sscanf(buffer, "%255[^:]:%255[^\n] %s", name,model)!= 2)){
		       continue;
		}
		int i = strlen(name) - 1;
		while (i >= 0 && (name[i] == ' ' || name[i] == '\t')) {
			name[i] = '\0';
			i--;
		}

		if (strcmp(name, "model name") == 0){

	
		printf("cpu: %s\n", model);
		break;
		
		}

		
	

	}
	fclose(file);
	}
	
	return 0;

}



const static void disk_usage(void){

	struct statvfs buf;

	if (statvfs("/" ,&buf) !=0){
		printf("perror");
		
	}

	unsigned long long total =(unsigned long long)buf.f_blocks * buf.f_frsize;
	unsigned long long free =(unsigned long long)buf.f_bfree * buf.f_frsize;
	
	double gb = 1024 * 1024 * 1024;

	unsigned long long used = total - free;

	total = total / gb;
	used = used / gb;

	printf("disk usage : %ld gb / %ld gb \n", used, total);

}

int ker_ver(void){

	FILE *file;

	char buffer_1[256];

	file =(popen("uname -r", "r"));

	if(file){

		if(fgets(buffer_1, sizeof(buffer_1), file)){

		printf("kernel version :%s", buffer_1);
		
		}

		pclose(file);

	}

	return 0;
}

void mem_usage(){

	FILE *file;

	char buffer[256];

	long mem_avail = 0;

	char line[256];

	int mem_used = 0;

	int mem_total = 0;

	file = fopen("/proc/meminfo" ,"r");

	if (file == NULL){}

	while (fgets(line, sizeof(line), file)) {

    char name[64];
    long value = 0;

    if (sscanf(line, "%63[^:]: %ld kB", name, &value) != 2) {
        continue;
    }

    if (strcmp(name, "MemTotal") == 0) {
        mem_used += value;
        mem_total = value;
    }
    else if (strcmp(name, "Shmem") == 0) {
        mem_used += value;
    }
    else if (strcmp(name, "MemFree") == 0 ||
             strcmp(name, "Buffers") == 0 ||
             strcmp(name, "Cached") == 0 ||
             strcmp(name, "SReclaimable") == 0) {
        mem_used -= value;
    }
    else if (strcmp(name, "MemAvailable") == 0) {
        mem_avail = value;
    }
}

    fclose(file);

    if (mem_avail) {
        mem_used = (mem_total - mem_avail) / 1024;
    }
    else {
        mem_used /= 1024;
    }

    mem_total /= 1024;

    printf("RAM: %ld MiB / %ld MiB\n", mem_used, mem_total);
}

static const int host_name(void){

    FILE *file;

    char buffer[64];

    file =fopen("/etc/hostname", "r");

    if (file == NULL){
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), file)){
        printf("hostname :%s", buffer);
    }
    fclose(file);
    
    return 0;
}

static const int user_name(void){
    FILE *file;

    char buffer[64];

    file=popen("echo $USER", "r");

    fgets(buffer , sizeof(buffer), file);

    printf("username :%s", buffer);

    pclose(file);

    return 0;
}
static int uptime(){
    FILE *file;

    file = popen("uptime -p", "r");

    char buffer[64];

    fgets(buffer, sizeof(buffer), file);

    printf("uptime :%s", buffer);

    pclose(file);

    return 0;



}
int main(void)
{
    
    
    user_name();
    host_name();
    uptime();
    mem_usage();
    ker_ver();
    cpu_info();
    disk_usage();


    return 0;
}

