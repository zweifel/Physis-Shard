
y <- runif(1:10000,min=0,max=180)

#exponential
x <- rexp(1:10000,3)
k2 <- cbind(x*sin(y),x*cos(y))

#uniform
x2 <- runif(1:10000,min=0,max=3)
k <- cbind(x2*sin(y),x2*cos(y))

write.table(k2,"circular_exp",row.names=F,col.names=F)
write.table(k,"circular_uniform",row.names=F,col.names=F)
