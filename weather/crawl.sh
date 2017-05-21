#!/bin/bash

ROOT=$(pwd)
CRAWL=${ROOT}/weather
CMD="scrapy crawl weather"
cd ${CRAWL}
${CMD}
cd -

