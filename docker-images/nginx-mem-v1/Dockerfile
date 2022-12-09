FROM nginx:alpine

RUN apk add python3
COPY entrypoint.sh .

ENTRYPOINT ["/entrypoint.sh"]
