#task 1
res <- read.csv(file='lead_concentration.csv', encoding = "UTF-8")
res1=boxplot(res,xlab="year",ylab="Concentration")


#task 2
# 1977 has an overall higher lead concentration
#and in 1976 the fluctuation of concentration seems a lot bigger, based on Q3-Q1

#task 3
res1976=boxplot(res$Fall.1976)
res1976=res1976$out
res1977=boxplot(res$Fall.1977)
res1977=res1977$out

length(res$Fall.1977)
mean1=(sum(res$Fall.1976)-sum(res1976))/(length(res$Fall.1976)-length(res1976))
mean2=(sum(na.omit(res$Fall.1977))-sum(res1977))/(length(na.omit(res$Fall.1977))-length(res1977))

# the mean for 1976 is 7.0, and the mean for 1977 is 9.1