####################
#Training the model#
####################
library(gbm)
train_data <- read.csv("/home/abhay/personalizedWebSearchChallenge/data/ClassifierRequired/Result_allQueries/feature_train.txt",head=F,sep=",")

gc(reset=T)
system.time(gbmModel<-gbm( train_data$V23 ~ .
                           , data = train_data
                           , distribution="multinomial"
                           , n.trees=100
                           , interaction.depth = 8
                           , n.minobsinnode = 20
                           , shrinkage = 0.1
                           , bag.fraction =0.5
                           ,train.fraction = 0.8,
                           , verbose=TRUE))

#save(gbmModel,file=paste0('gbm_model_1.RData'))

test_data <- read.csv("/home/abhay/personalizedWebSearchChallenge/data/ClassifierRequired/Result_allQueries/feature_test.txt",head=F,sep=",")

results<-predict(gbmModel,newdata = test_data,n.trees=gbmModel$n.trees,type="response")

write(results, 
	file = "predicted_result",
      ncolumns = 1,
      append = FALSE)
