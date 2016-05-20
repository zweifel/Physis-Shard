
prefix=''

repetitions <- 30
#sum <- read.table(as.character(1))
sum <- read.table(paste(prefix,1,sep=""))
for(i in 2:repetitions)
{
	#print(i)
	#a <- read.table(as.character(i))
	a <- read.table(paste(prefix,i,sep=""))
	sum <- a + sum
}

sum <- sum/repetitions

write.table(sum,"average~normal.txt",col.names=F,row.names=F)
