df <- read.csv2(file='./orig_exp_short',header=FALSE)
colnames(df) <- c("Rabbits","Foxes")
plot(df[1:5000,],main="t=1 to 5000",pch='.')
dev.copy(postscript, "1_to_5000.ps")
plot(df[100:5000,],main="t=100 to 5000",pch='.')
plot(df[100:50000,],main="t=100 to 50000",pch='.')
plot(df[100:250000,],main="t=100 to 250000",pch='.')
dev.off()

