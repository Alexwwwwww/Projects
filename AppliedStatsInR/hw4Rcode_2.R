a<-read.table("concrete_data.csv", head=T,sep=',',fileEncoding = 'UTF-8')
a
total=c(a$X1,a$X2,a$X3,a$X4,a$X5)
group=as.factor(rep(c("A","B","C","D","E"),each=6))
oneway.test(total~group)