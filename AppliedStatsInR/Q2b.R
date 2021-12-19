x<-c(2.1,5.3,0.9,4.6,1.4)
y<-c(1.9,0.5,3.1,2.8)
t.test(x,y,alternative="greater",var.equal=TRUE,conf.level=0.95)