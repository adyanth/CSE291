FROM nginx:alpine

RUN apk add python3
COPY entrypoint.sh .
COPY busy.py .

ENTRYPOINT ["/entrypoint.sh"]
