test:
	shellcheck *.bash
	docker build -t roryrjb/ngx-uname .
	docker run -it --rm roryrjb/ngx-uname

.PHONY: test
