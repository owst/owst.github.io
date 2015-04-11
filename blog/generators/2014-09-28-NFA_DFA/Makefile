DATE=2014-09-28
NAME=$(DATE)-NFA_DFA

$(NAME).md : $(NAME).source MakeImages.pl
	rm -f *.svg 
	echo "Processing $<"
	perl MakeImages.pl $< > $(NAME).md
	cp $(NAME).md ../../_posts
	cp *.svg ../../../assets/images/blog/$(DATE)
