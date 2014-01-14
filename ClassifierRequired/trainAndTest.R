################
# NDCG function#
################

ndcg <- function(x) {
  # x is a vector of relevance scores
  ideal_x <- rev(sort(x))
  DCG <- function(y) y[1] + sum(y[-1]/log(2:length(y), base = 2))
  DCG(x)/DCG(ideal_x)
}

####################
#Training the model#
####################
library(gbm)
gc(reset=T)
load(file = "processTrainM.RData");

system.time(gbmModel<-gbm( label ~ .
                           , data = processTrainM
                           #, distribution="bernoulli"
                           , distribution = list(   # loss function:
                             name='pairwise',       # pairwise
                             metric="ndcg",         # ranking metric: normalized discounted cumulative gain
                             group='sid',
                             max.rank=10), 
                           , n.trees=100
                           , interaction.depth = 8
                           , n.minobsinnode = 20
                           , shrinkage = 0.1
                           , bag.fraction =0.5
                           , train.fraction = 0.8,
                           , verbose=TRUE))

#load(file = "processTest.RData");
results<-predict(gbmModel,processTest,n.trees=gbmModel$n.trees,type="response")

sub<-cbind(processTest["sid"],processTest["url_id"], results)
r_rank<-ave(-sub$results, sub$sid, FUN=rank)
sub<-cbind(sub,r_rank)
fs<-sub[order(sub$sid, sub$r_rank), ]
fs<-fs[,c(1,2)]

write.table(fs, file = "sub", sep = ",",append=F,row.names=FALSE)
