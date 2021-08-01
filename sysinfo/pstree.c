#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_NODES 1024

/* a node on the tree */
struct node {
	int pid;
	char* name;
	int parentPid;
	int nextChild;
	int childrenIndex[1024];
};
struct node tree[MAX_NODES];
unsigned int nextNode = 0;
int initIndex = -1; /* where the init(PID=1) process is */

void drawTree()
{
	printf("draw init index: %d\n", initIndex);
	struct node initNode = tree[initIndex];
	printf("[%d] %s\n", initNode.pid, initNode.name);
	if (initNode.nextChild == 0)
		return;
	int childNode;
	for (int i=0; i<initNode.nextChild; i++) {
		childNode = initNode.childrenIndex[i];
		printf("|- %d\n", childNode);
	}
}

int main()
{
	DIR *dir = opendir("/proc/");
	if (dir == NULL) {
		perror("opendir error");
		exit(EXIT_FAILURE);
	}
	/* enumerate for directories in /proc */
	for (struct dirent *rdir = readdir(dir); rdir != NULL; rdir = readdir(dir)) {
		char *exess;
		int pidNum = (int) strtol(rdir->d_name, &exess, 10);
		/* if the directory is not a PID directory */
		if (exess[0] != '\0') {
			continue;
		}
		/* read stuff from status file */
		char *pid = rdir->d_name;
		char filename[strlen("/proc/") + strlen(pid) + strlen("/status") + 1];
		filename[0] = '\0';
		strcat(filename, "/proc/");
		strcat(filename, pid);
		strcat(filename, "/status");
		int fd = open(filename, O_RDONLY);
		/* the file might not exist now */
		if (fd == -1)
			continue;
		lseek(fd, 0, SEEK_SET);
		char *name = malloc(2048);
		char buf[65536];
		buf[65535] = '\0';
		for (int readed = read(fd, buf, 65536); readed > 0; readed = read(fd, buf, 65536)) {
			/* find the name of the command this process is running */
			for (int i=0; i<65536; i++) {
				if (buf[i] != '\t')
					continue;
				int idx = 0;
				for (int j=i+1; j<65536; j++) {
					if (buf[j] == '\n')
						break;
					memcpy(name+idx, buf+j, 1);
					//name[idx] = buf[j];
					idx++;
				}
				name[idx] = '\0';
				break;
			}
			/* find this process's parent PID */
			char *location = strstr(buf, "PPid:\t");
			if (location == NULL) {
				printf("could not find ppid\n");
				continue;
			}
			int idx = 0;
			location = location + 6;
			for (; *location != '\n'; location++) {
				buf[idx] = *location;
				idx++;
				buf[idx] = '\0';
			}
		}
		int ppid = atoi(buf);
		/* create node for this process */
		tree[nextNode].name = name;
		tree[nextNode].pid = pidNum;
		tree[nextNode].parentPid = ppid;
		tree[nextNode].nextChild = 0;
		if (pidNum == 1) {
			initIndex = nextNode;
		}
		/* if this a child of init, update init node */
		if (initIndex != -1 && ppid == 1) {
			tree[initIndex].childrenIndex[tree[initIndex].nextChild] = nextNode;
			memcpy(&tree[initIndex].childrenIndex[tree[initIndex].nextChild], &nextNode, sizeof(nextNode));
			tree[initIndex].nextChild++;
		}
		nextNode++;
	}
	drawTree();
}
