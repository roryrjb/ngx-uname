#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <sys/utsname.h>

#define JSON "{\n\
  \"sysname\": \"%s\",\n\
  \"nodename\": \"%s\",\n\
  \"release\": \"%s\",\n\
  \"version\": \"%s\",\n\
  \"machine\": \"%s\"\n\
}\n"

static char *ngx_http_uname(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_http_uname_commands[] = {
  {
    ngx_string("uname"),
    NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
    ngx_http_uname,
    0,
    0,
    NULL
  },

  ngx_null_command
};

static ngx_http_module_t ngx_http_uname_module_ctx = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

ngx_module_t ngx_http_uname_module = {
  NGX_MODULE_V1,
  &ngx_http_uname_module_ctx,
  ngx_http_uname_commands,
  NGX_HTTP_MODULE,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_uname_handler(ngx_http_request_t *r) {
  ngx_buf_t *b;
  ngx_chain_t out;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_type.len = sizeof("application/json") - 1;
  r->headers_out.content_type.data = (u_char *) "application/json";

  struct utsname uts;
  uname(&uts);

  char output[] = JSON;

  size_t size = sizeof(output) + sizeof(uts.sysname) + sizeof(uts.nodename) +
    sizeof(uts.release) + sizeof(uts.version) + sizeof(uts.machine);

  b = ngx_create_temp_buf(r->pool, size);

  if (b == NULL) {
    return NGX_HTTP_INTERNAL_SERVER_ERROR;
  }

  out.buf = b;
  out.next = NULL;

  b->last = ngx_sprintf(b->last, output, uts.sysname, uts.nodename,
    uts.release, uts.version, uts.machine);

  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.content_length_n = b->last - b->pos;
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}

static char *ngx_http_uname(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  ngx_http_core_loc_conf_t *clcf;

  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_uname_handler;

  return NGX_CONF_OK;
}
