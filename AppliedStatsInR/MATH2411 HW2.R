x<-c(46.4,46.1,45.7,47.0,46.1,45.9,45.3,46.9,45.2,46.2)
SampleMean<-mean(x)
#print(SampleMean)
#as an estimator of mu
SampleVariance<-var(x)
#since we only have sample variance, the estimation should be using z method
sd<-sqrt(SampleVariance)
t=-qt((1-0.93)/2,df=9)
leftbound=SampleMean-t*sd/sqrt(10)
rightbound=SampleMean+t*sd/sqrt(10)
#print(leftbound)
#print(rightbound)
cat("The 93% C.I.for mean is [",leftbound,",",rightbound,"]")



#calculate the critical value using qchisq function in R
rcritical<-qchisq(0.5+0.95/2,df=9,lower.tail = FALSE)
lcritical<-qchisq(0.5-0.95/2,df=9,lower.tail = FALSE)
varianceLB<-9*SampleVariance/(lcritical)
varianceUB<-9*SampleVariance/(rcritical)                                                                                                                                                                            
SDLB<-sqrt(varianceLB)
SDUB<-sqrt(varianceUB)
#estimation for the bound of 1/sigma:
cat("The 95% C.I.for 1/¦Ò is [",1/SDUB,",",1/SDLB,"]")


