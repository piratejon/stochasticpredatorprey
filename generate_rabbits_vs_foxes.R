df <- read.csv2(file='./orig_exp_short',header=FALSE)
colnames(df) <- c("Rabbits","Foxes")
plot(df[1:5000,],main="t=1 to 5000")
dev.copy(postscript, "1_to_5000.ps")
plot(df[100:5000,],main="t=100 to 5000")
dev.off()

