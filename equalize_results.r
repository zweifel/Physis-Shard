

prefix=''

repetitions <- 30
#sum <- read.table(as.character(1))
for(i in 1:repetitions)
{
	#print(i)
	#a <- read.table(as.character(i))
	a <- read.table(paste(prefix,i,sep=""))
	#print(a)
	k <- length(a[,1])
	
	if(k == 2000)
	{
		write.table(a,paste("p",i,sep=""),row.names=F,col.names=F)
	}
	else
	{
		a[k,] <- c((k)*100-100,100001)
		x <- seq((k+1)*100-100,200000 -100,by=100)
		y <- as.data.frame(cbind(x,100001))
		names(y) <- c("x","y")
		names(a) <- c("x","y")
		completedata <- rbind(a,y)
		#print(completedata)
	
		write.table(completedata,file=paste("p",i,sep=""),row.names=F,col.names=F)
	}


}



