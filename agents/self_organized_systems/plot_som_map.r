
require(grDevices)

args <- commandArgs(trailingOnly = TRUE)
#print(args[1])

#data_name <- read.table("processing_som_map")
data_name <- args[1]


#for( i in 0:999)
#{


#	a <- read.table(paste("som_map",i,sep=""))

a <- read.table(paste(data_name[1]))

jpg_name <- paste(data_name[1],".jpg",sep="")

jpeg(jpg_name)

size <- sqrt(nrow(a))

#a <- read.table("test")

sqrt(nrow(a))

#convert from code coordinates to correct plot coordinates
		#b <- cbind(a[,1],a[nrow(a):1,2])
#b <- cbind(a[,1],abs(a[,2]-(size+1)))

b <- cbind(a[,1],(a[,2]))

plot(b)#,cex=(a[,3]/100))

for( i in 1:(nrow(b)-1))
{
	if(i%%size!=0)
	{
		segments(b[i,1],b[i,2],b[i+1,1],b[i+1,2])
	}
}

for( i in 0:(size-1))
{
	#print(i)
	for(j in seq(1,(nrow(b)-size-i),by=size))
	{
		#print(paste("i",i,"j",j))
		segments(b[j+i,1],b[j+i,2],b[j+size+i,1],b[j+size+i,2])
		
	}

}

#plot(a[,1],a[nrow(a):1,2],cex=(a[,3]/100))

#plot(a[nrow(a):1,2],a[,1],cex=(a[,3]/100))
#plot(a[,2],a[,1],col=heat.colors(a[,3]))
#plot(a[,1],a[nrow(a):1,2],col=heat.colors(a[,3]))
#plot(a[nrow(a):1,2],a[,1],cex=(a[,3]))
#plot(a[nrow(a):1,2],a[,1],col=heat.colors(a[,3]))

dev.off()
